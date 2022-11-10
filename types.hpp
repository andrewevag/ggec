

class Entity {};

class Labelt : public Entity {};

class Type : public Entity {};

class Boolt : public Type {};

class Intt : public Type {};

class Voidt : public Type {};

class Chart : public Type {};

class Doublet : public Type {};

class Pointert : public Type {
	Pointert(Type* of) : _of(of) {}
private:
	Type* _of;
};

class Anyt : public Type {};



class TypedEntity {

protected:
	Type* _t = nullptr;
	bool _isLVAL = false;
};