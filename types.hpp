#include <vector>

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
};

class Boolt : public Type {
public:
	virtual size_t getSizeInBytes() override {
		return 1;
	}
};

class Intt : public Type {
public:
	virtual size_t getSizeInBytes() override {
		return 2;
	}
};

class Voidt : public Type {
public:
	virtual size_t getSizeInBytes() override {
		return 0;
	}
};

class Chart : public Type {
	public:
	virtual size_t getSizeInBytes() override {
		return 1;
	}
};

class Doublet : public Type {
public:
	virtual size_t getSizeInBytes() override {
		return 10;
	}
};

class Pointert : public Type {
public:
	Pointert(Type* of) : _of(of) {}
	virtual size_t getSizeInBytes() override {
		return 8; //llvm needs 8
	}
private:
	Type* _of;
};

class Anyt : public Type {
public:
	virtual size_t getSizeInBytes() override {
		return 0; // should never get called
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

protected:
	Type* _t = nullptr;
	bool _isLVAL = false;
};