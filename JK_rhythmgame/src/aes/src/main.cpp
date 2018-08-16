#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <exception>
#include "../include/key.hpp"
#include "../include/aes.hpp"

int main() {
	using std::cout;
	using std::endl;
	
	std::string pass = "jfodsckcxvjoelwrhldld";
	auto k = enc::makeKey(pass);
	enc::aes crypto{ k };

	std::ifstream ifs("README.md", std::ios::binary);
	std::ofstream ofs("encrypt.encrypt", std::ios::binary);
	if(ifs && ifs.is_open() && ofs && ofs.is_open()) crypto.encrypt(ifs, ofs);
	else return 0;

	ifs.close();
	ofs.close();

	ifs.open("encrypt.encrypt", std::ios::binary);
	ofs.open("README_2.md", std::ios::binary);

	try{
	if(ifs && ifs.is_open() && ofs && ofs.is_open()) crypto.decrypt(ifs, ofs);
	} catch (std::exception e) {cout << e.what();}
	return 0;
}


