/*
 * mle.h
 *
 *  Created on: Mar 25, 2019
 *      Author: luis
 */

#ifndef MLE_H_
#define MLE_H_

double mle_alpha(reading *readings, int nReadings);
double score_function(double alpha, void *args);
double bic_score_function(double alpha, void *args);

#endif /* MLE_H_ */
