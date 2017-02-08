#include "simpletest.h"
#include "calcula.h"


void test_calcula1(){

	DESCRIBE("calcula sequencial");
	WHEN("I execute calcula");
	IF("use the file arq1.txt and the substring 'an' ");
	THEN("the result should be 0");
	isEqual(ocorrencias("arq1.txt","an", 1), 0);

	WHEN("I execute calcula");
	IF("use the file papper.txt and the substring 'for' ");
	THEN("the result should be 646");
	isEqual(ocorrencias("papper.txt","for", 1), 646);

	WHEN("I execute calcula");
	IF("use the file arq1.txt and the substring 'DASH' ");
	THEN("the result should be 13");
	isEqual(ocorrencias("arq1.txt","DASH", 1), 13);

	WHEN("I execute calcula");
	IF("use the file arq1.txt and the substring ' ' ");
	THEN("the result should be 40832");
	isEqual(ocorrencias("papper.txt"," ", 1), 40832);
}


int main(){
	test_calcula1();
}