// SPH.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <vector>
#include <cmath>
#include <math.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "Particle.h"

#define PI 3.14159265359
#define T 293.15
#define H 0.1
#define DT 0.03


using namespace std;

vector<Particle> particleArray;
int step;
double SpikyKernel(vec3 r , double h)
{
  double factor = (double)15 / (PI * pow(h, 6));
  double f2 = pow(h - r.getMagnitude(), 3);
  return factor * f2;
}

double SpikyGradient(double r, double h)
{
  double f1 = (double)-45 / (PI * pow(h, 6));
  double f2 = (double)(h - r)*(h - r); // BE CAREFULL, ADD SIGN IF NECESSARY

  return f1 * f2;
}

double SpikyLaplacian(double r, double h)
{
  double f1 = (double)-90 / (PI*pow(h, 6));
  double f2 = (double)(h - r);
  double f3 = (double)(h - 2*r);
  return f1 * f2*f3;
}

double Poly6Kernel(double r, double h)
{
  double factor = (double)315 / (64 * PI*pow(h, 9));
  double f2 = (double)r*pow(h*h - r * r, 3);
  return factor * f2;
}

double Poly6Gradient(double r, double h)
{
  double f1 = (double)-945 / (32 * PI*pow(h, 9));
  double f2 = r * pow(h*h - r * r, 2);
  return f1 * f2;
}

double Poly6Laplacian(double r, double h)
{
  double f1 = (double)-945 / (32 * PI*pow(h, 9));
  double f2 = h * h - r * r;
  double f3 = 3 * h*h - 7 * r*r;
  return f1 * f2*f3;
}

double ViscosityLagrangian(double r, double h)
{
  double f1 = (double)45 / (r*pow(h, 6));
  double f2 = h - r;

  return abs(f1*f2);
}
struct int2
{
  int start;
  int finish;
};
void* calculateDensity(void* ceo)
{

  
    int2* co = (int2*) ceo;
     for (int i = co->start; i < co->finish; i++)
    {
     
       //particleArray[i].neighbors.clear();
      particleArray[i].density = 1;
      particleArray[i].Fpressure = vec3(0, 0, 0);
      particleArray[i].FGravity = vec3(0, 0, 0);
      particleArray[i].Fviscocity = vec3(0, 0, 0);
      

          for (int j = 0; j < particleArray.size(); j++)
	{
	  if (i == j)
	    continue;
	  double X, Y, Z;

	  X = particleArray[i].position.x;
	  Y = particleArray[i].position.y;
	  Z = particleArray[i].position.z;

	  double distSquared = (particleArray[j].position.x - X)*(particleArray[j].position.x - X) +
	    (particleArray[j].position.y - Y)*(particleArray[j].position.y - Y) +
	    (particleArray[j].position.z - Z)*(particleArray[j].position.z - Z);

	  double tolerance = H;

	  if (distSquared < tolerance*tolerance) {
	    //cout << "i: "<<i << " j: " << j << endl;
	    particleArray[i].neighbors.push_back(&particleArray[j]);

	    // Density Computation:
	    float h = H;
	    float h8 = pow(h, 8);
	    float C = 4 * particleArray[i].mass / (PI * h8);
	    float dx = particleArray[i].position.x - particleArray[j].position.x;
	    float dy = particleArray[i].position.y - particleArray[j].position.y;
	    float dz = particleArray[i].position.z - particleArray[j].position.z;
	    float distsq = dx * dx + dy * dy + dz * dz;
	    float z = h * h - distsq;
	    if (z > 0)
	      {
		float hi = z * z*z*C;
		//particleArray[i].density += hi;
		//particleArray[j].density += hi;
	      }
	    particleArray[i].density += particleArray[j].mass * Poly6Kernel(sqrt(distSquared), H);
	    //cout << "density: " << i<<" "<< (particleArray[j].position.y )<<" "<<particleArray[i].position.y<<" " << distSquared<<endl;
	    //cout << "Kernel: " << Poly6Kernel(sqrt(distSquared), H)<<endl;

	  }

	  }


            particleArray[i].pressure = 10* (particleArray[i].density - 1);//cout << particleArray[i].pressure << endl;
    }
  
}

void* calculateFlux(void* ceo)
{
  int2* co = (int2*) ceo;
  for (int i = co->start; i < co->finish; i++)
    {
      //cout << particleArray[i].neighbors.size();

      vec3 rij;
      for (int j = 0; j < particleArray[i].neighbors.size(); j++)
	{
	  Particle ne = *particleArray[i].neighbors[j];
	  rij = particleArray[i].position - ne.position;


	  vec3 no = rij.Normalized();

	  float CO = particleArray[i].mass / PI / pow(H, 4);
	  float Cp = 50; // k
	  float Cv = -40; // viscocity


	  vec3 d = particleArray[i].position - ne.position;
	  float r = d.getMagnitude();
	  //if (r < H)
	  //{
	  float density = ne.density;
	  float q = r / H;
	  float u = 1 - q;
	  float w0 = CO * u / particleArray[i].density / density;
	  float wp = w0 * Cp*(density + particleArray[i].density - 2)*u / q;
	  float wv = w0 * Cv;
	  vec3 dv = particleArray[i].velocity - ne.velocity;
	  particleArray[i].acceleration.x += (wp*d.x + wv * d.x);
	  particleArray[i].acceleration.y += (wp*d.y + wv * d.y);
	  particleArray[i].acceleration.z += (wp*d.z + wv * d.z);

	  //}













	  /*vec3 fp;
	        fp.x -= ne.mass * (ne.pressure / ne.density)*SpikyGradient(rij.getMagnitude(), H)*no.x;
		    fp.y -= ne.mass * (ne.pressure / ne.density)*SpikyGradient(rij.getMagnitude(), H)*no.y;
		        fp.z -= ne.mass * (ne.pressure / ne.density)*SpikyGradient(rij.getMagnitude(), H)*no.z;

			//cout << fp.x<<" "<<fp.y<<" "<<fp.z << endl;
			    particleArray[i].Fpressure += fp;

			        vec3 fv;
				    fv.x += ne.mass * (ne.velocity.x / ne.density)*ViscosityLagrangian(rij.getMagnitude(), H)*no.x;
				        fv.y += ne.mass * (ne.velocity.y / ne.density)*ViscosityLagrangian(rij.getMagnitude(), H)*no.y;
					    fv.z += ne.mass * (ne.velocity.z / ne.density)*ViscosityLagrangian(rij.getMagnitude(), H)*no.z;

					        particleArray[i].Fviscocity += fv;
						//cout <<" Spiky: "<< SpikyGradient(rij.getMagnitude(), 1) << endl;


						    }
						        particleArray[i].FGravity.y = -10.0*particleArray[i].density;
							//cout << particleArray[i].FGravity.y<<endl;

							    particleArray[i].acceleration = particleArray[i].FGravity + particleArray[i].Fpressure;//+particleArray[i].Fviscocity;

							    particleArray[i].acceleration /= particleArray[i].density;*/


	}

      particleArray[i].acceleration.y -= 10;
    }
}

void* LeapFrogIntegration(void* ceo)
{
  int2* co =( int2*) ceo;
  for (int i = co->start; i < co->finish; i++)
    {

      if (step == 0)
	{

	  particleArray[i].halfVelocity = particleArray[i].velocity + particleArray[i].acceleration*0.5*DT;
	  particleArray[i].velocity += particleArray[i].acceleration*DT;

	  particleArray[i].position += particleArray[i].halfVelocity * DT;


	}
      else
	{
	  particleArray[i].halfVelocity += particleArray[i].acceleration*DT;
	  particleArray[i].velocity += particleArray[i].halfVelocity + particleArray[i].acceleration*0.5*DT;

	  particleArray[i].position += particleArray[i].halfVelocity*DT;

	  //cout << particleArray[i].acceleration.y << " ";
	}

      float bound = 3;
      // From Bindell
      float DAMP = 0.3;
      if (particleArray[i].position.y <= -bound)
	{
	  float tbounce = (particleArray[i].position.y + bound) / particleArray[i].velocity.y;
	  particleArray[i].position -= particleArray[i].velocity * (1 - DAMP)*tbounce;

	  particleArray[i].position.y = 2 * (-bound) - particleArray[i].position.y;
	  particleArray[i].velocity.y = -particleArray[i].velocity.y;
	  particleArray[i].halfVelocity.y = -particleArray[i].halfVelocity.y;

	  particleArray[i].velocity = particleArray[i].velocity* DAMP;
	  particleArray[i].halfVelocity = particleArray[i].halfVelocity* DAMP;


	}

      if (particleArray[i].position.y >= bound)
	{
	  float tbounce = (particleArray[i].position.y - bound) / particleArray[i].velocity.y;
	  particleArray[i].position -= particleArray[i].velocity * (1 - DAMP)*tbounce;

	  particleArray[i].position.y = 2 * (bound)-particleArray[i].position.y;
	  particleArray[i].velocity.y = -particleArray[i].velocity.y;
	  particleArray[i].halfVelocity.y = -particleArray[i].halfVelocity.y;

	  particleArray[i].velocity = particleArray[i].velocity* DAMP;
	  particleArray[i].halfVelocity = particleArray[i].halfVelocity* DAMP;


	}

      bound = 3.0f;
      if (particleArray[i].position.x <= -bound)
	{
	  float tbounce = (particleArray[i].position.x + bound) / particleArray[i].velocity.x;
	  particleArray[i].position -= particleArray[i].velocity * (1 - DAMP)*tbounce;

	  particleArray[i].position.x = 2 * (-bound) - particleArray[i].position.x;
	  particleArray[i].velocity.x = -particleArray[i].velocity.x;
	  particleArray[i].halfVelocity.x = -particleArray[i].halfVelocity.x;

	  particleArray[i].velocity = particleArray[i].velocity* DAMP;
	  particleArray[i].halfVelocity = particleArray[i].halfVelocity* DAMP;


	}
      if (particleArray[i].position.x >= bound)
	{
	  float tbounce = (particleArray[i].position.x - bound) / particleArray[i].velocity.x;
	  particleArray[i].position -= particleArray[i].velocity * (1 - DAMP)*tbounce;

	  particleArray[i].position.x = 2 * (bound)-particleArray[i].position.x;
	  particleArray[i].velocity.x = -particleArray[i].velocity.x;
	  particleArray[i].halfVelocity.x = -particleArray[i].halfVelocity.x;

	  particleArray[i].velocity = particleArray[i].velocity* DAMP;
	  particleArray[i].halfVelocity = particleArray[i].halfVelocity* DAMP;


	}
      bound =3.0f;
      if (particleArray[i].position.z <= -bound)
	{
	  float tbounce = (particleArray[i].position.x + bound) / particleArray[i].velocity.z;
	  particleArray[i].position -= particleArray[i].velocity * (1 - DAMP)*tbounce;

	  particleArray[i].position.z = 2 * (-bound) - particleArray[i].position.z;
	  particleArray[i].velocity.z = -particleArray[i].velocity.z;
	  particleArray[i].halfVelocity.z = -particleArray[i].halfVelocity.z;

	  particleArray[i].velocity = particleArray[i].velocity* DAMP;
	  particleArray[i].halfVelocity = particleArray[i].halfVelocity* DAMP;


	}
      if (particleArray[i].position.z >= bound)
	{
	  float tbounce = (particleArray[i].position.z - bound) / particleArray[i].velocity.z;
	  particleArray[i].position -= particleArray[i].velocity * (1 - DAMP)*tbounce;

	  particleArray[i].position.z = 2 * (bound)-particleArray[i].position.z;
	  particleArray[i].velocity.z = -particleArray[i].velocity.z;
	  particleArray[i].halfVelocity.z = -particleArray[i].halfVelocity.z;

	  particleArray[i].velocity = particleArray[i].velocity* DAMP;
	  particleArray[i].halfVelocity = particleArray[i].halfVelocity* DAMP;


	}

      /* if (particleArray[i].position.x < -1000 || particleArray[i].position.y < -1000 || particleArray[i].position.y < -1000)
	{
	  particleArray[i].position = vec3(0, 0, 0);
	  particleArray[i].velocity = vec3(0, 0, 0);
	  particleArray[i].halfVelocity = vec3(0, 0, 0);
	}
      */

    }

}
int main(int argc , char** argv)
{
  int threadCount = atoi(argv[1]);
  
  pthread_t threads[threadCount];

  ofstream file;
  file.open("output.txt");
  
  for (int y = 0; y < 15; y++)
    for (int x = 0; x < 15; x++)
      for (int z = 0; z < 15; z++)
	particleArray.push_back(Particle(x*0.15f*sin(x), y*0.1f*sin(y), z*0.1f*sin(z), 0.01, 20));

  int pL = particleArray.size();
  int2 co[4];
  
  for (step = 0; step < 100; step++)
    {// find neighbors

          for (int i = 0; i < threadCount; i++)
	{

	  co[i].start = i * pL/threadCount;
	  co[i].finish = co[i].start + pL / threadCount;
	  //	  printf("start %d f %d" , co[i].start , co[i].finish);
	  pthread_create(&threads[i], NULL, calculateDensity, &co[i]);
	}

	        for (int i = 0; i < threadCount; i++)
	  	pthread_join(threads[i], NULL);
		
      for (int i = 0; i < threadCount; i++)
	{

	  co[i].start = i * pL/threadCount;
	  co[i].finish = co[i].start + pL / threadCount;
	  pthread_create(&threads[i], NULL, calculateFlux, &co[i]);
	}

      for (int i = 0; i < threadCount; i++)
	pthread_join(threads[i], NULL);

      for (int i = 0; i < threadCount; i++)
	{

	  co[i].start = i * pL/threadCount;
	  co[i].finish = co[i].start + pL / threadCount;
	  pthread_create(&threads[i], NULL, LeapFrogIntegration,&co[i]);
	}

      for (int i = 0; i < threadCount; i++)
	pthread_join(threads[i], NULL);
      
		
      cout << " gh "<<step << endl;
      cout << particleArray[0].position.x << " " << particleArray[0].position.y << " " << particleArray[0].position.z << endl;
      for (int i = 0; i < pL; i++) {
	file << i << " " << particleArray[i].position.x << " " << particleArray[i].position.y << " " << particleArray[i].position.z << endl;
	particleArray[i].neighbors.clear();
      }




    }
  file.close();

  return 0;
}
