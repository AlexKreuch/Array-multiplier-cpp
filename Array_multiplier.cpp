﻿// Array_multiplier.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <string>
#include <iostream>
using namespace std;

char itoc(int i) {
	char c = (char)(i + ((int)('0')));
	return c;
}
int ctoi(char c)
{
	int i = ((int)c) - ((int)('0'));
	return i;
}


bool is_numeral(char c) {
	return (int)c >= (int)'0' && (int)c <= (int)'9';
}

int check_string(string & x)
{
	/*
	   return : 
	     -1 : if the string does not encode a number
	      0 : if the string encodes the number 0
	      1 : if the string encodes a non-zero number
	*/

	if (x.length() == 0) return -1;
	if (x.length() == 1)
	{
		char c = x.at(0);
		if (!is_numeral(c)) return -1;
		return c == '0' ? 0 : 1;
	}

	bool first_non_zero_found = false;

	int index = 0;

	if (x.at(0) == '-') index++;

	while (index < x.length())
	{
		if (!is_numeral(x.at(index))) return -1;
		if (x.at(index) != '0') first_non_zero_found = true;
		index++;
	}

	return first_non_zero_found ? 1 : 0;
	
}

struct STATE {
	string * s1;
	string * s2;
	char * tape;
	int p1;
	int p2;
	int p3;
	int reset_point; // only the second can be reset

public:
	STATE(string & s_1, string & s_2, int n)
	{
		s1 = &s_1;
		s2 = &s_2;
		tape = new char[n]; for (int i = 0; i < n - 1; i++) tape[i] = '0'; tape[n - 1] = '\0';
		p1 = s1->length() - 1;
		p2 = s2->length() - 1;
		p3 = n - 2;
		reset_point = p3;
	}

	bool cpe1() { return check_past_end_util(s1, p1); } // check if s1 is past the end
	bool cpe2() { return check_past_end_util(s2, p2); } // check if s2 is past the end
	void mu1() { move_up_util(s1,&p1,true); }
	void mu2() { move_up_util(s2,&p2,false); }
	int get1() { return get_util(1); };
	int get2() { return get_util(2); };
	int get3() { return get_util(3); };
	void put(int i) { char c = itoc(i % 10); tape[p3] = c; }
	void reset_2nd() {
		p2 = s2->length()-1;
		p3 = reset_point;
	}
	string finish(bool is_negative) {
		int index = 0;
		while (tape[index] == '0') index++;
		if (is_negative) tape[--index] = '-';
		char * out = &tape[index];
		return (string)out;
	}

private:
	bool check_past_end_util(string * s, int p) {
		if (p < 0) return true;
		return s->at(p) == '-';
	}
	void move_up_util(string * s, int * p, bool num1)
	{
		if (check_past_end_util(s, p[0])) return;
		p[0]--;
		p3--;
		if (num1) reset_point--;
	}
	int get_util(int index) {
		char c = '0';
		switch (index)
		{
			case 1: c = s1[0].at(p1); break;
			case 2: c = s2[0].at(p2); break;
			case 3: c = tape[p3]; break;
		}
		return ctoi(c);
	}
};

string compute(bool is_negative, STATE state) { 
	
	while (!state.cpe1())
	{
		#pragma region computeStep
		int carry = 0;
		while (!state.cpe2())
		{
			int v1 = state.get1();
			int v2 = state.get2();
			int v3 = state.get3();
			int v4 = (v1*v2) + v3 + carry;
			state.put(v4 % 10);
			carry = v4 / 10;
			state.mu2();
		}
		if (carry != 0)
		{
			int v = state.get3();
			v = v + carry;
			state.put(v);
		}
		#pragma endregion
		state.reset_2nd();
		state.mu1();
	}

	string output = state.finish(is_negative);

	return output;
}

string multiplier(string & s1, string & s2)
{
	int code1 = check_string(s1);
	int code2 = check_string(s2);
	if (code1 == -1 || code2 == -1) return "<invalid_input>";
	if (code1*code2 == 0) return "0";
	bool flag1 = s1.at(0) == '-';
	bool flag2 = s2.at(0) == '-';
	bool is_negative = flag1 xor flag2;
	int len = s1.length() + s2.length() + 3;
	STATE state = STATE(s1, s2, len);
	string result = compute(is_negative, state);
	return result;
}

string prompt_and_get(string msg)
{
	string input = "";
	cout << msg;
	cin >> input;
	return input;
}



int main()
{
	const string doneCode = "DONE";
	while (true)
	{
		string a = prompt_and_get("enter 1st number : ");
		if (a == doneCode) break;
		string b = prompt_and_get("enter 2nd number : ");
		if (b == doneCode) break;
		string c = multiplier(a, b);
		if (c != "<invalid_input>")
		{
			c = "(" + a + ")x(" + b + ") = " + c;
		}
		cout << c << '\n';
	}
	cout << "goodbye";
    return 0;
}

