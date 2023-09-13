// Author: Marc Comino 2020

#include "glwidget.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "./mesh_io.h"
#include "./triangle_mesh.h"
#include "./particlemanager.h"

using namespace std;

namespace {

const double kFieldOfView = 80;
const double kZNear = 0.0001;
const double kZFar = 10000;

const char kPhongVertexShaderFile[] = "../shaders/phong.vert";
const char kPhongFragmentShaderFile[] = "../shaders/phong.frag";
const char kSkyVertexShaderFile[] = "../shaders/sky.vert";
const char kSkyFragmentShaderFile[] = "../shaders/sky.frag";

const int kVertexAttributeIdx = 0;
const int kNormalAttributeIdx = 1;

/*** FRAMERATE ***/
int FPS = 0;
float myFPS = 0.0f;
time_t iniTime;
time_t endTime;


bool ReadFile(const std::string filename, std::string *shader_source) {
  std::ifstream infile(filename.c_str());

  if (!infile.is_open() || !infile.good()) {
    std::cerr << "Error " + filename + " not found." << std::endl;
    return false;
  }

  std::stringstream stream;
  stream << infile.rdbuf();
  infile.close();

  *shader_source = stream.str();
  return true;
}


bool LoadImage(const std::string &path, GLuint cube_map_pos) {
  QImage image;
  bool res = image.load(path.c_str());
  if (res) {
    QImage gl_image = image.mirrored();
    glTexImage2D(cube_map_pos, 0, GL_RGBA, image.width(), image.height(), 0,
                 GL_BGRA, GL_UNSIGNED_BYTE, image.bits());
  }
  return res;
}

bool LoadProgram(const std::string &vertex, const std::string &fragment, QOpenGLShaderProgram *program) {
  std::string vertex_shader, fragment_shader;
  bool res = ReadFile(vertex, &vertex_shader) && ReadFile(fragment, &fragment_shader);

  if (res) {
    program->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                     vertex_shader.c_str());
    program->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                     fragment_shader.c_str());
    program->bindAttributeLocation("vertex", kVertexAttributeIdx);
    program->bindAttributeLocation("normal", kNormalAttributeIdx);
    program->link();
  }

  return res;
}

bool LoadCubeMap(const QString &dir) {
  std::string path = dir.toUtf8().constData();
  bool res = LoadImage(path + "/right.png", GL_TEXTURE_CUBE_MAP_POSITIVE_X);
  res = res && LoadImage(path + "/left.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
  res = res && LoadImage(path + "/top.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
  res = res && LoadImage(path + "/bottom.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
  res = res && LoadImage(path + "/back.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
  res = res && LoadImage(path + "/front.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

  if (res)
  {
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  }

  return res;
}

}  // namespace


//Particle Manager.
static ParticleManager PartMan;

ParticleSystem ps_;

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent), initialized_(false), num_instances(10), width_(0.0), height_(0.0), dist_offset(1.0), myLod(0),
      my_method( "Euler (Original)" ), upd_method( Particle::UpdateMethod::EulerOrig ), psType( ParticleSystem::ParticleSystemType::Fountain ),
      file("../models/sphere.ply")
{
  setFocusPolicy(Qt::StrongFocus);
  iniTime = time( NULL );
  VAO = std::vector< GLuint >(10);
  vbo_v_id = std::vector< GLuint >(10);
  vbo_n_id = std::vector< GLuint >(10);
  faces_id = std::vector< GLuint >(10);

  ps_.setParticleSystem( num_instances );
}

GLWidget::~GLWidget() {}

bool GLWidget::LoadModel(const QString &filename) {
  /*std::string*/ file = filename.toUtf8().constData();
  size_t pos = file.find_last_of(".");
  std::string type = file.substr(pos + 1);

  std::unique_ptr<data_representation::TriangleMesh> mesh =
      std::make_unique<data_representation::TriangleMesh>();

  bool res = false;
  if (type.compare("ply") == 0) {
    res = data_representation::ReadFromPly(file, mesh.get());
  }

  if (res) {
    mesh_.reset(mesh.release());
    camera_.UpdateModel(mesh_->min_, mesh_->max_);

    int N = PartMan.computeParticlePositions( mesh_->vertices_, mesh_->faces_, mesh_->normals_,   mesh_->min_, mesh_->max_, my_method );

    // TODO(students): Create / Initialize buffers.
    int i = 0;
    // Create & bind empty VAO
    glGenVertexArrays(1, &VAO[i]);
    glBindVertexArray(VAO[i]);

    // Initialize VBO for vertices
    glGenBuffers(1, &vbo_v_id[i]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_v_id[0]);
    glBufferData(GL_ARRAY_BUFFER, PartMan.vtxPerLOD[0].size() * sizeof(float), &PartMan.vtxPerLOD[0][0], GL_STATIC_DRAW);
    glVertexAttribPointer(kVertexAttributeIdx, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(kVertexAttributeIdx);


    // Initialize VBO for normals
    glGenBuffers(1, &vbo_n_id[i]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_n_id[i]);
    glBufferData(GL_ARRAY_BUFFER, PartMan.normPerLOD[0].size() * sizeof(float), &PartMan.normPerLOD[0][0], GL_STATIC_DRAW);
    glVertexAttribPointer(kNormalAttributeIdx, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(kNormalAttributeIdx);

    glBindVertexArray(0);

    // Initialize VBO for faces
    glGenBuffers(1, &faces_id[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faces_id[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, PartMan.facesPerLOD[0].size() * sizeof(int), &PartMan.facesPerLOD[0][0], GL_STATIC_DRAW);


    emit SetFaces(QString(std::to_string(PartMan.facesPerLOD[0].size() / 3).c_str()));
    emit SetVertices(QString(std::to_string(PartMan.vtxPerLOD[0].size() / 3).c_str()));
    // END.
    return true;
  }
  return false;
}


bool GLWidget::LoadSphere( ) {
  std::string sphFile = "../models/sphere_r1.ply";
  size_t pos = sphFile.find_last_of(".");
  std::string type = sphFile.substr(pos + 1);

  std::unique_ptr<data_representation::TriangleMesh> mesh2 =
      std::make_unique<data_representation::TriangleMesh>();

  bool res = false;
  if (type.compare("ply") == 0) {
    res = data_representation::ReadFromPly(sphFile, mesh2.get());
  }

  if (res) {
    mesh2_.reset(mesh2.release());

    // Create & bind empty VAO
    glGenVertexArrays(1, &VAO_sph);
    glBindVertexArray(VAO_sph);

    // Initialize VBO for vertices
    glGenBuffers(1, &vbo_v_id_sph);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_v_id_sph);
    glBufferData(GL_ARRAY_BUFFER, mesh2_->vertices_.size() * sizeof(float), &mesh2_->vertices_[0], GL_STATIC_DRAW);
    glVertexAttribPointer(kVertexAttributeIdx, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(kVertexAttributeIdx);


    // Initialize VBO for normals
    glGenBuffers(1, &vbo_n_id_sph);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_n_id_sph);
    glBufferData(GL_ARRAY_BUFFER, mesh2_->normals_.size() * sizeof(float), &mesh2_->normals_[0], GL_STATIC_DRAW);
    glVertexAttribPointer(kNormalAttributeIdx, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(kNormalAttributeIdx);

    glBindVertexArray(0);

    // Initialize VBO for faces
    glGenBuffers(1, &faces_id_sph);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faces_id_sph);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh2_->faces_.size() * sizeof(int), &mesh2_->faces_[0], GL_STATIC_DRAW);

    // END.
    return true;
  }
  return false;
}

void GLWidget::initializeGL() {
  glewInit();

  glEnable(GL_NORMALIZE);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);

  phong_program_ = std::make_unique<QOpenGLShaderProgram>();
  sky_program_ = std::make_unique<QOpenGLShaderProgram>();

  bool res = LoadProgram(kPhongVertexShaderFile, kPhongFragmentShaderFile,
                         phong_program_.get());
  res = res && LoadCubeMap( QString( "../textures/box/" ) );
  res = res && LoadProgram(kSkyVertexShaderFile, kSkyFragmentShaderFile,
                           sky_program_.get());

  if (!res) exit(0);

  LoadModel("../models/sphere.ply");
  LoadSphere();

  initialized_ = true;
}

void GLWidget::resizeGL(int w, int h) {
  if (h == 0) h = 1;
  width_ = w;
  height_ = h;

  camera_.SetViewport(10, 0, w, h);
  camera_.SetProjection(kFieldOfView, kZNear, kZFar);
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    camera_.StartRotating(event->x(), event->y());
  }
  if (event->button() == Qt::RightButton) {
    camera_.StartZooming(event->x(), event->y());
  }
  updateGL();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  camera_.SetRotationX(event->y());
  camera_.SetRotationY(event->x());
  camera_.SafeZoom(event->y());
  updateGL();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    camera_.StopRotating(event->x(), event->y());
  }
  if (event->button() == Qt::RightButton) {
    camera_.StopZooming(event->x(), event->y());
  }
  updateGL();
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Up) camera_.Zoom(-10);
  if (event->key() == Qt::Key_Down) camera_.Zoom(10);
  if (event->key() == Qt::Key_Left) camera_.Rotate(-1);
  if (event->key() == Qt::Key_Right) camera_.Rotate(1);

  if (event->key() == Qt::Key_W) camera_.Move( 1, 0);
  if (event->key() == Qt::Key_S) camera_.Move(-1, 0);
  if (event->key() == Qt::Key_A) camera_.Move( 0,-1);
  if (event->key() == Qt::Key_D) camera_.Move( 0, 1);

  if (event->key() == Qt::Key_Z) camera_.Zoom(-10);
  if (event->key() == Qt::Key_X) camera_.Zoom(10);

  if (event->key() == Qt::Key_Q) camera_.Rotate(-1);
  if (event->key() == Qt::Key_E) camera_.Rotate(1);

  if (event->key() == Qt::Key_R)
  {
    ps_.setParticleSystem( num_instances );

    phong_program_.reset();
    phong_program_ = std::make_unique<QOpenGLShaderProgram>();
    LoadProgram(kPhongVertexShaderFile, kPhongFragmentShaderFile,
                phong_program_.get());
    sky_program_.reset();
    sky_program_ = std::make_unique<QOpenGLShaderProgram>();
    LoadProgram(kSkyVertexShaderFile, kSkyFragmentShaderFile,
                sky_program_.get());
  }

  updateGL();
}

void GLWidget::paintGL() {
  glClearColor(0.05f, 0.33f, 0.37f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (initialized_) {
    camera_.SetViewport();

    Eigen::Matrix4f projection = camera_.SetProjection();
    Eigen::Matrix4f view = camera_.SetView();
    Eigen::Matrix4f model = camera_.SetModel();

    Eigen::Matrix4f t = view * model;
    Eigen::Matrix3f normal;
    for (int i = 0; i < 3; ++i)
      for (int j = 0; j < 3; ++j) normal(i, j) = t(i, j);

    normal = normal.inverse().transpose();

    if (mesh_ != nullptr) {
      GLint projection_location, view_location, model_location, normal_matrix_location, numinst_location, offset_location;

      phong_program_->bind();
      projection_location = phong_program_->uniformLocation("projection");
      view_location = phong_program_->uniformLocation("view");
      model_location = phong_program_->uniformLocation("model");
      normal_matrix_location = phong_program_->uniformLocation("normal_matrix");
      numinst_location = phong_program_->uniformLocation("num_instances");
      offset_location = phong_program_->uniformLocation("offset");

      glUniformMatrix4fv(projection_location, 1, GL_FALSE, projection.data());
      glUniformMatrix4fv(view_location, 1, GL_FALSE, view.data());
      glUniformMatrix4fv(model_location, 1, GL_FALSE, model.data());
      glUniformMatrix3fv(normal_matrix_location, 1, GL_FALSE, normal.data());
      glUniform1i(numinst_location, 1 );


      // Framerate counter
      ++FPS;
      endTime = time( NULL );
      if ( endTime > iniTime ) {
          myFPS = FPS / (endTime - iniTime);
          emit SetFramerate(QString( std::to_string( myFPS ).c_str() ));
          iniTime = endTime;
          FPS = 0;
      }

      for( int i = 0; i < (int) num_instances ; ++i)
      {
          glm::vec3 myPart = ps_.getParticle( i ).getCurrentPosition();
          glUniform3f(offset_location, myPart.x, myPart.y, myPart.z );

          // TODO(students): Implement model rendering.
          glBindVertexArray(VAO[ myLod ]);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faces_id[ myLod ]);

          glDrawElements(GL_TRIANGLES, mesh_->faces_.size(), GL_UNSIGNED_INT, 0);
          //glDrawElementsInstanced(GL_TRIANGLES, PartMan.facesPerLOD[ myLod ].size(), GL_UNSIGNED_INT, 0, num_instances * num_instances);

          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
          glBindVertexArray(0);
      }

// /////////////// Box BEGIN

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
      // skybox VAO
      GLfloat SB = 6.0f; //SIZE
      GLfloat skyboxVertices[] = { // positions
         -SB,  SB, -SB,
         -SB, -SB, -SB,
          SB, -SB, -SB,
          SB,  SB, -SB,
         -SB, -SB,  SB,
         -SB,  SB,  SB,
          SB, -SB,  SB,
          SB,  SB,  SB,

          // custom tri
           5, -3,  1 ,
           2,  1, -4 ,
           0, -1, -2
      };

      GLubyte skyboxIndices[] = { // indices per face
          0, 1, 2,  2, 3, 0,
          4, 1, 0,  0, 5, 4,
          2, 6, 7,  7, 3, 2,
          4, 5, 7,  7, 6, 4,
          //0, 3, 7,  7, 5, 0, no top
          1, 4, 2,  2, 4, 6

          // custom tri,
         //,8, 9, 10
      };

      glGenVertexArrays(1, &skyboxVAO);
      glBindVertexArray(skyboxVAO);
      glGenBuffers(1, &skyboxVBO);

      glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
      glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(0);


      glGenBuffers(1, &skyIndexVBO);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyIndexVBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxIndices, GL_STATIC_DRAW);


      // TODO(students): implement the rendering of a bounding cube displaying the
      // environment map.
      sky_program_->bind();
       projection_location = sky_program_->uniformLocation("projection");
       view_location = sky_program_->uniformLocation("view");
       model_location = sky_program_->uniformLocation("model");
       normal_matrix_location = sky_program_->uniformLocation("normal_matrix");
      GLuint specular_map_location = sky_program_->uniformLocation("specular_map");

      glUniformMatrix4fv(projection_location, 1, GL_FALSE, projection.data());
      glUniformMatrix4fv(view_location, 1, GL_FALSE, view.data());
      glUniformMatrix4fv(model_location, 1, GL_FALSE, model.data());
      glUniformMatrix3fv(normal_matrix_location, 1, GL_FALSE, normal.data());

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_CUBE_MAP, specular_map_);
      glUniform1i(specular_map_location, 0);

      // TODO(students): implement the rendering of a bounding cube displaying the
      // environment map.
      glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
      // skybox cube
      glBindVertexArray(skyboxVAO);
      glActiveTexture(GL_TEXTURE0);
      glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_BYTE, (void*) 0);
      glBindVertexArray(0);
      glDepthFunc(GL_LESS);

// /////////////// Box END + Sphere
    if (mesh2_ != nullptr)
    {
          Eigen::Matrix4f sca;
          sca <<  20,  0,  0,  0,
                  0,  20,  0,  0,
                  0,   0, 20,  0,
                  0,   0,  0,  1;
          Eigen::Matrix4f modelSca = sca*camera_.SetModel()  ;

          phong_program_->bind();
          projection_location = phong_program_->uniformLocation("projection");
          view_location = phong_program_->uniformLocation("view");
          model_location = phong_program_->uniformLocation("model");
          normal_matrix_location = phong_program_->uniformLocation("normal_matrix");
          numinst_location = phong_program_->uniformLocation("num_instances");
          offset_location = phong_program_->uniformLocation("offset");

          glUniformMatrix4fv(projection_location, 1, GL_FALSE, projection.data());
          glUniformMatrix4fv(view_location, 1, GL_FALSE, view.data());
          glUniformMatrix4fv(model_location, 1, GL_FALSE, modelSca.data());
          glUniformMatrix3fv(normal_matrix_location, 1, GL_FALSE, normal.data());
          glUniform1i(numinst_location, 1 );


          glUniform3f(offset_location, 0, -0.2, 0 );

          // TODO(students): Implement model rendering.
          glBindVertexArray(VAO[ myLod ]);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faces_id[ myLod ]);

          glDrawElements(GL_TRIANGLES, mesh_->faces_.size(), GL_UNSIGNED_INT, 0);
          //glDrawElementsInstanced(GL_TRIANGLES, PartMan.facesPerLOD[ myLod ].size(), GL_UNSIGNED_INT, 0, num_instances * num_instances);

          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
          glBindVertexArray(0);
    }

// ////////////////////// MODEL PAINTING END
      if (myFPS == 0.0f)
          myFPS = 1.0f;
      float RATE = 1.0f;
      ps_.updateParticleSystem( RATE*0.1f, upd_method ); // ::EulerOrig | ::EulerSemi | ::Verlet

      emit SetFaces(    QString(std::to_string(PartMan.facesPerLOD[ myLod ].size() / 3).c_str()) );
      emit SetVertices( QString(std::to_string(PartMan.vtxPerLOD[ myLod ].size()   / 3).c_str()) );
      // END.
    }

    model = camera_.SetIdentity();
  }
}

void GLWidget::SetNumInstances(int numInst) {
    num_instances = numInst;
    std::cout << "There are " << numInst << " particles\n";
    ps_.setParticleSystem( num_instances, psType );
    updateGL();
}

void GLWidget::SetDistanceOffset(double offset){
    dist_offset = (float) offset;
    updateGL();
}

void GLWidget::SetLevelOfDetail(int myLod) {
    myLod = myLod - 1;
    if (myLod == 1)
    {
        std::cout << "Particle System set to: FOUNTAIN\n";
        psType = ParticleSystem::ParticleSystemType::Fountain;
    }
    else if (myLod == 2)
    {
        std::cout << "Particle System set to: WATERFALL\n";
        psType = ParticleSystem::ParticleSystemType::Waterfall;
    }
    ps_.setParticleSystem( num_instances, psType );
    updateGL();
}


void GLWidget::SetMethod(QString method) {
    my_method = method.toUtf8().constData();
    if (my_method == "Euler (Original)")
    {
        std::cout << "Method = [" << my_method << "]\n";
        upd_method = Particle::UpdateMethod::EulerOrig;
    }
    else if (my_method == "Euler (Semi)")
    {
        std::cout << "Method = [" << my_method << "]\n";
        upd_method = Particle::UpdateMethod::EulerSemi;
    }
    else if (my_method == "Verlet")
    {
        std::cout << "Method = [" << my_method << "]\n";
        upd_method = Particle::UpdateMethod::Verlet;
    }
    updateGL();
}




// they all reset the particle system!!!

void GLWidget::SetDamping(double val)
{
    ps_.setSpringDamping( (float) val );
    ps_.setParticleSystem( num_instances, psType );
    updateGL();
}


void GLWidget::SetElasticity(double val)
{
    ps_.setSpringElasticity( (float) val );
    ps_.setParticleSystem( num_instances, psType );
    updateGL();
}


void GLWidget::SetLength(double val)
{
    ps_.setSpringLength( (float) val );
    ps_.setParticleSystem( num_instances, psType );
    updateGL();
}
