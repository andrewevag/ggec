#include <vector>
#include "ast.hpp"

class Entity {
public:
	virtual size_t getSizeInBytes() = 0;
};

class Labelt : public Entity {
public:
	virtual size_t getSizeInBytes() override {
		return 0;
	}
};

class Type : public Entity {
public:
	virtual size_t getSizeInBytes() = 0;
	virtual Type* copy() = 0;
};

class Boolt : public Type {
public:
	virtual size_t getSizeInBytes() override {
		return 1;
	}
	virtual Type* copy() override {
		return new Boolt();
	};
};

class Intt : public Type {
public:
	virtual size_t getSizeInBytes() override {
		return 2;
	}
	virtual Type* copy() override {
		return new Intt();
	};
};

class Voidt : public Type {
public:
	virtual size_t getSizeInBytes() override {
		return 0;
	}
	virtual Type* copy() override {
		return new Voidt();
	};
};

class Chart : public Type {
	public:
	virtual size_t getSizeInBytes() override {
		return 1;
	}
	virtual Type* copy() override {
		return new Chart();
	}
};

class Doublet : public Type {
public:
	virtual size_t getSizeInBytes() override {
		return 10;
	}
	virtual Type* copy() override {
		return new Doublet();
	}
};

class Pointert : public Type {
public:
	Pointert(Type* of) : _of(of) {}
	virtual size_t getSizeInBytes() override {
		return 8; //llvm needs 8
	}

	virtual Type* copy() override {
		return new Pointert(this->_of.copy());
	}
private:
	Type* _of;
};

class Anyt : public Type {
public:
	virtual size_t getSizeInBytes() override {
		return 0; // should never get called
	}
	virtual Type* copy() override {
		return new Anyt();
	}
};

class PassingWay {
public:
	enum PassingWayEnum { ByCall, ByRef };
	PassingWayEnum _way;
};



class Functiont : public Type {
public:
	virtual size_t getSizeInBytes() override {
		return 0; // should never get called
	}
private:
	Type* _resultType;
	std::vector<std::pair<PassingWay, Type*> > _parameterTypes;
};


class TypedEntity {
public:
	virtual Type* getType() = 0;
protected:
	Type* _t = nullptr;
	bool _isLVAL = false;
};