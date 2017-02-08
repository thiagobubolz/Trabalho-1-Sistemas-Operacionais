#include "simpletest.h"
#include "calculat.h"


void test_calcula1(){

	DESCRIBE("calcula threads");
	WHEN("I execute calcula");
	IF("use the file arq1.txt and the substring 'an' ");
	THEN("the result should be 0");
	isEqual(ocorrencias("arq1.txt","an", 3), 0);

	WHEN("I execute calcula");
	IF("use the file papper.txt and the substring 'for' ");
	THEN("the result should be 646");
	isEqual(ocorrencias("papper.txt","for", 3), 646);

	WHEN("I execute calcula");
	IF("use the file arq1.txt and the substring 'DASH' ");
	THEN("the result should be 13");
	isEqual(ocorrencias("arq1.txt","DASH", 3), 13);

	WHEN("I execute calcula");
	IF("use the file arq1.txt and the substring ' ' ");
	THEN("the result should be 40832");
	isEqual(ocorrencias("papper.txt"," ", 3), 40832);
}

int main(){
	test_calcula1();
}