/**
 * particle_filter.cpp
 *
 * Created on: Dec 12, 2016
 * Author: Tiffany Huang
 */

#include "particle_filter.h"

#include <math.h>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "helper_functions.h"

using std::string;
using std::vector;


void ParticleFilter::init(double x, double y, double theta, double std[]) {
  /**
   * TODO: Set the number of particles. Initialize all particles to 
   *   first position (based on estimates of x, y, theta and their uncertainties
   *   from GPS) and all weights to 1. 
   * TODO: Add random Gaussian noise to each particle.
   * NOTE: Consult particle_filter.h for more information about this method 
   *   (and others in this file).
   */
  std::default_random_engine gen;
  num_particles = 10;  // TODO: Set the number of particles

  std::normal_distribution<double> dist_x(x, std[0]);
  std::normal_distribution<double> dist_y(y, std[1]);
  std::normal_distribution<double> dist_theta(theta, std[2]);  
  for (int i = 0; i < num_particles; ++i) {
    Particle P; 
    P.id=i;
    P.x=dist_x(gen);
    P.y=dist_y(gen);
    P.theta=dist_theta(gen);
    P.weight=1.0;

    particles.push_back(P);
	  weights.push_back(P.weight);
  }
  is_initialized=true;
}

void ParticleFilter::prediction(double delta_t, double std_pos[], 
                                double velocity, double yaw_rate) {
  /**
   * TODO: Add measurements to each particle and add random Gaussian noise.
   * NOTE: When adding noise you may find std::normal_distribution 
   *   and std::default_random_engine useful.
   *  http://en.cppreference.com/w/cpp/numeric/random/normal_distribution
   *  http://www.cplusplus.com/reference/random/default_random_engine/
   */

}

void ParticleFilter::dataAssociation(vector<LandmarkObs> predicted, 
                                     vector<LandmarkObs>& observations,
                                     double sensor_range) {
  /**
   * TODO: Find the predicted measurement that is closest to each 
   *   observed measurement and assign the observed measurement to this 
   *   particular landmark.
   * NOTE: this method will NOT be called by the grading code. But you will 
   *   probably find it useful to implement this method and use it as a helper 
   *   during the updateWeights phase.
   */
  double curr_dist = 0;
  
 	for (unsigned int i = 0; i<observations.size(); i++){
//    std::cout<<"OB,before:"<<observations[i].id<<std::endl;
		double min_dist = 10000;
		int closest_landmark_id=-1;

		for (unsigned int j = 0; j < predicted.size(); j++) {
			curr_dist = dist(observations[i].x,observations[i].y, predicted[j].x, predicted[j].y);

			if (curr_dist < min_dist){
				min_dist = curr_dist;
				closest_landmark_id = predicted[j].id;
			}
		}
		observations[i].id = closest_landmark_id;
 //   std::cout<<"OB,after:"<<observations[i].id<<" dis: "<<min_dist<<std::endl; 
	} 

}

void ParticleFilter::updateWeights(double sensor_range, double std_landmark[], 
                                   const vector<LandmarkObs> &observations, 
                                   const Map &map_landmarks) {
  /**
   * TODO: Update the weights of each particle using a mult-variate Gaussian 
   *   distribution. You can read more about this distribution here: 
   *   https://en.wikipedia.org/wiki/Multivariate_normal_distribution
   * NOTE: The observations are given in the VEHICLE'S coordinate system. 
   *   Your particles are located according to the MAP'S coordinate system. 
   *   You will need to transform between the two systems. Keep in mind that
   *   this transformation requires both rotation AND translation (but no scaling).
   *   The following is a good resource for the theory:
   *   https://www.willamette.edu/~gorr/classes/GeneralGraphics/Transforms/transforms2d.htm
   *   and the following is a good resource for the actual equation to implement
   *   (look at equation 3.33) http://planning.cs.uiuc.edu/node99.html
   */

 // For each particle....
  double weight_sum=0;
  for (unsigned int i = 0; i < particles.size(); ++i) {
//*** Step 1: Transform observations from vehicle's co-ordinate system 
//           to map co-ordinate sytem
    vector<LandmarkObs> transformed_observations;

		for (unsigned int j=0; j<observations.size(); ++j){
			LandmarkObs trans_obv;

			trans_obv.x = observations[j].x * cos(particles[i].theta) - observations[j].y * sin(particles[i].theta) + particles[i].x;
			trans_obv.y = observations[j].x * sin(particles[i].theta) + observations[j].y * cos(particles[i].theta) + particles[i].y;
			trans_obv.id = j;

			transformed_observations.push_back(trans_obv);
		}

/***Step 2: Keep only those landmarks which are in sensor range ***/
		vector<LandmarkObs> landmarks_In_Range;    
    for (unsigned int j = 0; j <map_landmarks.landmark_list.size(); j++) {
      double landmark_dist;
      landmark_dist = dist(particles[i].x, particles[i].y, map_landmarks.landmark_list[j].x_f, map_landmarks.landmark_list[j].y_f );
			if (landmark_dist<sensor_range){
				LandmarkObs landmark_item;
				landmark_item.id = map_landmarks.landmark_list[j].id_i;
				landmark_item.x = map_landmarks.landmark_list[j].x_f;
				landmark_item.y = map_landmarks.landmark_list[j].y_f;

				landmarks_In_Range.push_back(landmark_item);
			} 
    }

//    std::cout<<i<<"***: landmarks_In_Range:"<<landmarks_In_Range.size()<<std::endl;
//    std::cout<<"transformed_observations:"<<transformed_observations.size()<<std::endl;

    
/*** Step 3: Associate observations to predicted landmarks***/
		dataAssociation(landmarks_In_Range, transformed_observations, sensor_range);


/*Step 4: Calculate the weight of each particle using Multivariate Gaussian distribution.*/
    double sigma_x = std_landmark[0];
    double sigma_y = std_landmark[1];
    double sigma_x_2 = pow(sigma_x, 2);
    double sigma_y_2 = pow(sigma_y, 2);
    double normalizer = (1.0/(2.0 * M_PI * sigma_x * sigma_y));
    double weight_per_observation=1.0;


    for(unsigned int j = 0; j < transformed_observations.size(); ++j) {   
      weight_per_observation=1.0;

      double observationX = transformed_observations[j].x;
      double observationY = transformed_observations[j].y;

      int landmarkId = transformed_observations[j].id;
      double landmarkX, landmarkY;
      for(unsigned int k=0;k<landmarks_In_Range.size();++k ){
        if(landmarks_In_Range[k].id==landmarkId){
          landmarkX=landmarks_In_Range[k].x;
          landmarkY=landmarks_In_Range[k].y;
          break;
        }
      }
    double multi_prob = 1.0;
    double e_fact_1=pow((observationX - landmarkX), 2)/(2.0 * sigma_x_2);
    double e_fact_2=pow((observationY - landmarkY), 2)/(2.0 * sigma_y_2);
    multi_prob = normalizer * exp(-1.0 * ( e_fact_1+e_fact_2 ));
    
 //   std::cout<<j<<" landmark weight "<<multi_prob<<" n:"<<normalizer<<" "<<e_fact_1<<" "<<e_fact_2<<std::endl;
    weight_per_observation*=multi_prob;
    }
// get weight for each particle
  particles[i].weight=weight_per_observation;
  weight_sum+=weight_per_observation;
  std::cout<<i<<" particles "<<particles[i].x<<" "<<particles[i].y<<" "<<particles[i].theta <<" w="<<particles[i].weight<<std::endl;

  }
  std::cout<<" weight_sum "<<weight_sum<<std::endl;
  for(unsigned int i = 0; i < particles.size(); ++i){
    particles[i].weight=particles[i].weight/weight_sum;
    std::cout<<i<<" particles-norm "<<particles[i].weight<<std::endl;
  }
  mypause();  
}

void ParticleFilter::resample() {
  /**
   * TODO: Resample particles with replacement with probability proportional 
   *   to their weight. 
   * NOTE: You may find std::discrete_distribution helpful here.
   *   http://en.cppreference.com/w/cpp/numeric/random/discrete_distribution
   */

}

void ParticleFilter::SetAssociations(Particle& particle, 
                                     const vector<int>& associations, 
                                     const vector<double>& sense_x, 
                                     const vector<double>& sense_y) {
  // particle: the particle to which assign each listed association, 
  //   and association's (x,y) world coordinates mapping
  // associations: The landmark id that goes along with each listed association
  // sense_x: the associations x mapping already converted to world coordinates
  // sense_y: the associations y mapping already converted to world coordinates
  particle.associations= associations;
  particle.sense_x = sense_x;
  particle.sense_y = sense_y;
}

string ParticleFilter::getAssociations(Particle best) {
  vector<int> v = best.associations;
  std::stringstream ss;
  copy(v.begin(), v.end(), std::ostream_iterator<int>(ss, " "));
  string s = ss.str();
  s = s.substr(0, s.length()-1);  // get rid of the trailing space
  return s;
}

string ParticleFilter::getSenseCoord(Particle best, string coord) {
  vector<double> v;

  if (coord == "X") {
    v = best.sense_x;
  } else {
    v = best.sense_y;
  }

  std::stringstream ss;
  copy(v.begin(), v.end(), std::ostream_iterator<float>(ss, " "));
  string s = ss.str();
  s = s.substr(0, s.length()-1);  // get rid of the trailing space
  return s;
}