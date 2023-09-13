CC     = g++
CFLAGS = -I.
DEPS   = Plane.h Particle.h ParticleSystem.h
OBJ    = Plane.o Particle.o ParticleSystem.o particle_sample_main.o


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

partsim: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm *.o partsim
