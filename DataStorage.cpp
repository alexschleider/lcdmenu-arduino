#include "WProgram.h"

class DataType {
	public: enum eTypes {
		UNKNOWN,
		INT,
		FLOAT,
		CHAR,
		STRING,
		BOOLEAN,
		DOUBLE
	};
	
	template <class T> class Type {
	public:
		static const eTypes type = UNKNOWN;
	};
	
	public:
		DataType() {}
		template <class T> DataType(T data) { setType(data); }
		template <class T> void setType(T data) {
			dType = Type<T>::type;
		}
		int type() {
			return dType;
		}
	private:
		eTypes dType;
	
};

template <> class DataType::Type<int>{ public: static const eTypes type = DataType::INT; };
template <> class DataType::Type<float>{ public: static const eTypes type = DataType::FLOAT; };
template <> class DataType::Type<char>{ public: static const eTypes type = DataType::CHAR; };
template <> class DataType::Type<char*>{ public: static const eTypes type = DataType::STRING; };
template <> class DataType::Type<bool>{ public: static const eTypes type = DataType::BOOLEAN; };
template <> class DataType::Type<double>{ public: static const eTypes type = DataType::DOUBLE; };



class Setting {
public:
	Setting() {}
	template <class T> Setting(T &s) {
		init(s);
	}
	template <class T> Setting &operator =(T &s) {
		init(s);
		return (*this);
	}
	void set(void* p) {
		if(dataType.type() == DataType::INT) *((int*)(_setting)) = *((int*)(p));
		if(dataType.type() == DataType::FLOAT) *((float*)(_setting)) = *((float*)(p));
		if(dataType.type() == DataType::CHAR) *((char*)(_setting)) = *((char*)((const char*)(p)));
		if(dataType.type() == DataType::STRING) *((char**)(_setting)) = *((char**)((const char**)(p)));
		if(dataType.type() == DataType::BOOLEAN) *((bool*)(_setting)) = *((bool*)(p));
		if(dataType.type() == DataType::DOUBLE) *((double*)(_setting)) = *((double*)(p));
	}
	template <class T> boolean valueEquals(T t) {
		boolean ret = false;
		T *t1 = new T;
		T *t2 = new T;
		*t1 = t;
		*t2 = *(static_cast<T*>(_setting));
		if(*t1 == *t2) ret = true;
		delete t1;
		delete t2;
		return ret; 
	}
	template <class T> void init(T &s) {
		_setting = &s;
		dataType.setType(s);
	}
private:
	DataType dataType;
	void* _setting;
};