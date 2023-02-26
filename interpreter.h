#ifndef interpreter
#define interpreter

#include <iostream>
#include <vector>


class Block;

enum ReturnType {
    NONE_TYPE,
    PRIMITIVE_TYPE,
    OBJECT_TYPE,
	VAR_ASSIGNMENT_TYPE,
	FUNC_ASSIGNMENT_TYPE,
	BREAK_TYPE,
    RETURN_TYPE
};

enum ValueType {
	INT_TYPE,
	FLOAT_TYPE,
    BOOL_TYPE,
	STRING_TYPE,
	OBJECT_VALUE_TYPE // need to be implemented

};

class Scope {
    public:
    private:

};

class ValueObject {
    public:
    private:
};

class PrimitiveValue: public ValueObject {
    public:
        PrimitiveValue(std::string, ValueType);
        std::string getValue();
        ValueType getType();
    private:
        std::string value;
        ValueType type;
};

class ScopeValue: public ValueObject {
    public:
        ScopeValue(Scope*);
        Scope* getValue();
    private:
        Scope* scope;
};


class FuncInScope: public Scope {

    public:
        FuncInScope(std::string, std::vector<std::string>, Block*);
        std::string getName();
        std::vector<std::string> getParams();
        Block* getBody();
    private:
        std::string name;
        std::vector<std::string> param_names;
        Block* body;

};


class VarInScope: public Scope {
    public:
        VarInScope(std::string, std::string, ValueType);
        std::string getName();
        std::string getValue();
        ValueType getType();
    private:
        ValueType type;
        std::string name;
        std::string value;
};


class ReturnValue {
    public:
        ReturnValue(ReturnType, ValueObject*);
        ReturnValue(ReturnType);
        ReturnValue();
        ReturnType getType();
        ValueObject* getValueObj();

    private:
        ReturnType returnType;
        ValueObject* valueObj;

};

class VarScope {
    public:
        VarScope(std::vector<VarInScope*>);
        VarScope();
        bool isInScope(std::string);
        ReturnValue getValueByName(std::string);
        void push(VarInScope*);
        std::vector<VarInScope*> getScope();
    private:
        std::vector<VarInScope*> scope;
};

class FuncScope {
    public:
        FuncScope(std::vector<FuncInScope*>);
        FuncScope();
        bool isInScope(std::string, int);
        FuncInScope* getFuncByNameAndParamCount(std::string, int);
        void push(FuncInScope*);
        std::vector<FuncInScope*> getScope();
    private:
        std::vector<FuncInScope*> scope;
};


// Functions

std::string multIntString(std::string, std::string);
std::string multStringInt(std::string, std::string);
std::string multIntInt(std::string, std::string);
std::string minusIntInt(std::string, std::string);
std::string plusStringInt(std::string, std::string);
std::string plusIntString(std::string, std::string);
std::string plusIntInt(std::string, std::string);
std::string plusStringString(std::string, std::string);
std::string divideIntInt(std::string, std::string);
ReturnValue evalFuncBlock(VarScope&, VarScope&, FuncScope&, FuncScope&, Block*);
ReturnValue evalNonFuncBlock(VarScope&, VarScope&, FuncScope&, FuncScope&, Block*);

#endif