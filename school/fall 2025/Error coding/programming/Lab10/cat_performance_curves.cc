//  Program: testconvdec.cc
//
//  Todd K. Moon
//
// Copyright 2004 by Todd K. Moon
// Permission is granted to use this program/data
// for educational/research only

#include <iostream>
#include <cmath>
using namespace std;

extern "C" {
#include <stdio.h>
}

#include "BinConvFIR.h"
#include "BinConvdec01.h"
#include "BinConvdecBPSK.h"
#include "matalloc.h"
#include "BPSKmodvec.h"
#include <random>
#include <cmath>

int rand01() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> dist(0, 1);
    return dist(gen);
}

#include <fstream>
#include <iostream>

void write_csv(const std::string& filename, double* arr, int length) {
    std::ofstream file(filename);

    if(!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    for(int i = 0; i < length; i++) {
        file << arr[i];
        if(i != length - 1)
            file << ",";
    }

    file << "\n";
    file.close();
}


int main()
{
   int i;
   int decout;
	int start_db = 0;
	int end_db = 10;
	int db_diff = end_db - start_db;
	double uncoded_db_prob[db_diff] = {0};
	double hard_db_prob[db_diff] = {0};
	double soft_db_prob[db_diff] = {0};
	int N_errors = 50;
	int num_errors = 0;
	int num_trials = 0;
	BPSKmodvec modu(1);
	static std::mt19937 gen(std::random_device{}());
   static std::normal_distribution<double> dist(0.0, 1.0); // mean 0, std 1
	int out_bit;
	
	cout << "uncoded error prob" << endl;
	for(int i = start_db ; i < end_db; i++){
		double N0 = pow(10,-i/10.0);
		double sigma_2 = N0/2;
		double sigma = sqrt(sigma_2);
		num_errors = 0;
		num_trials = 0;
		// cout << "started trials" << endl;
		while(num_errors < N_errors){
			unsigned int bit[1] = {(unsigned int)rand01()};
			double* mod = modu.mod(bit);
			mod[0] += sigma*dist(gen);
			
			if(mod[0] > 0){
				out_bit = 1;
			} else{
				out_bit = 0;
			}
			// cout << "demodulated" << endl;
			if(out_bit != bit[0]){
				num_errors++;
			}
			num_trials++;
		}
		// cout << "exited the trials" << endl;
		uncoded_db_prob[i] = ((double)num_errors)/((double)num_trials);
		// cout << "num_errors = " << num_errors << " num_trials = " << num_trials << endl;
		cout << "db = " << i << " probability of error is " << uncoded_db_prob[i] << endl;
	}

	unsigned int **h;
	int k = 1;
	int n = 2;
	CALLOCMATRIX(h, unsigned int, k, n);
	h[0][0] = 3; h[0][1] = 5;
	int p[] = {2};
	unsigned int data_in[8];
	unsigned int data_encoded[16];
	double data_enc_mod[16];
	unsigned int data_dec_out[8];
	unsigned int outs[2];
	
	BinConvFIR conv(k, n, p, h);
	BinConvdec01 decoder(conv,8);
	BPSKmodvec modulator(n);
	cout << endl << endl << "hard decision error prob" << endl;
	for(int i = start_db ; i < end_db; i++){
		double N0 = pow(10,-i/10.0);
		double sigma_2 = N0/2;
		double sigma = sqrt(sigma_2);
		num_errors = 0;
		num_trials = 0;
		// cout << "started trials" << endl;
		while(num_errors < N_errors){
			for(int j = 0; j < 8;j++){
				unsigned char d[1];
				 if (j < 6)
					d[0] = (unsigned char)rand01();
				 else
					d[0] = 0;  // tail bits
				data_in[j] = (unsigned int) d[0];
				unsigned int* out = conv.encode(&d[0]);
				data_encoded[j] = out[0];
				data_encoded[j+1] = out[1];
				double* mod = modulator.mod(out);
				data_enc_mod[j] = mod[0];
				data_enc_mod[j+1] = mod[1];
				mod[0] += sigma*dist(gen);
				mod[1] += sigma*dist(gen);
				
				if(mod[0] > 0){
					outs[0] = 1;
				} else{
					outs[0] = 0;
				}
				
				if(mod[1] > 0){
					outs[1] = 1;
				} else{
					outs[1] = 0;
				}
				
				decout = decoder.decode(outs);
			}
			// conv.setstate(0);
			int j = 0;
			while(decoder.getinpnow(1)){
				data_dec_out[j] = decoder.inputs;
				if(data_dec_out[j] != data_in[j])
					num_errors++;
				num_trials++;
				j++;
			}	
			data_dec_out[7] = decoder.inputs;
			if(data_dec_out[7] != data_in[7])
				num_errors++;
			num_trials++;
			
			// for(j = 0; j < 8; j++){
				// cout << "in " << data_in[j] << " data out " << data_dec_out[j] << endl;
			// }
			// decoder.setstartstate(0);
		}
		// cout << "exited the trials" << endl;
		hard_db_prob[i] = ((double)num_errors)/((double)num_trials);
		// cout << "num_errors = " << num_errors << " num_trials = " << num_trials << endl;
		cout << "db = " << i << " probability of error is " << hard_db_prob[i] << endl;
	}
	
	BinConvdecBPSK decoderBSPK(conv,8);
	conv.setstate(0);
	cout << endl << endl << "soft decision error prob" << endl;
	for(int i = start_db ; i < end_db; i++){
		double N0 = pow(10,-i/10.0);
		double sigma_2 = N0/2;
		double sigma = sqrt(sigma_2);
		num_errors = 0;
		num_trials = 0;
		// cout << "started trials" << endl;
		while(num_errors < N_errors){
			for(int j = 0; j < 8;j++){
				unsigned char d[1];
				 if (j < 6)
					d[0] = (unsigned char)rand01();
				 else
					d[0] = 0;  // tail bits
				data_in[j] = (unsigned int) d[0];
				unsigned int* out = conv.encode(&d[0]);
				data_encoded[j] = out[0];
				data_encoded[j+1] = out[1];
				double* mod = modulator.mod(out);
				data_enc_mod[j] = mod[0];
				data_enc_mod[j+1] = mod[1];
				mod[0] += sigma*dist(gen);
				mod[1] += sigma*dist(gen);
				
				decout = decoderBSPK.decode(mod);
			}
			// conv.setstate(0);
			int j = 0;
			while(decoderBSPK.getinpnow(1)){
				data_dec_out[j] = decoderBSPK.inputs;
				if(data_dec_out[j] != data_in[j])
					num_errors++;
				num_trials++;
				j++;
			}	
			data_dec_out[7] = decoderBSPK.inputs;
			if(data_dec_out[7] != data_in[7])
				num_errors++;
			num_trials++;
			
			// for(j = 0; j < 8; j++){
				// cout << "in " << data_in[j] << " data out " << data_dec_out[j] << endl;
			// }
			// decoder.setstartstate(0);
		}
		// cout << "exited the trials" << endl;
		soft_db_prob[i] = ((double)num_errors)/((double)num_trials);
		// cout << "num_errors = " << num_errors << " num_trials = " << num_trials << endl;
		cout << "db = " << i << " probability of error is " << soft_db_prob[i] << endl;
	}
	
	write_csv("uncoded2_per_curve.csv", uncoded_db_prob, end_db - start_db);
	write_csv("hard2_per_curve.csv", hard_db_prob, end_db - start_db);
	write_csv("soft2_per_curve.csv", soft_db_prob, end_db - start_db);

}


/*
Local Variables:
compile-command: "g++ -o cat_performance -g cat_performance_curves.cc Convdec.cc BinConvFIR.cc BinConvdec01.cc BinConvdecBPSK.cc"
End:
*/
