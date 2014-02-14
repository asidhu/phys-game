#pragma once

template <class T>
class autoptr{
private:
	int ref;
	T* data;
	bool alive;
	void kill(){
		alive = false;
	}
public:
	T* getdata(){ return data; }
	bool isAlive(){ return alive; }
	autoptr(T* input){
		ref = 0;
		data = input;
		alive = true;
	}
	void get(){
		ref++;
	}
	void free(){
		ref--;
		if (ref == 0)
			delete this;
	}
	friend T;
};