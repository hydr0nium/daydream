#ifndef interpreter
#define interpreter

#include <iostream>
#include <vector>


class Block;

enum ReturnType {
	NONE_TYPE,
	INT_TYPE,
	FLOAT_TYPE,
    BOOL_TYPE,
	STRING_TYPE,
	OBJECT_TYPE, // need to be implemented
	VAR_ASSIGNMENT_TYPE,
	FUNC_ASSIGNMENT_TYPE,
	BREAK_TYPE,
    RETURN_TYPE
};

class Scope {
    public:
    private:

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
        VarInScope(std::string, std::string, ReturnType);
        std::string getName();
        std::string getValue();
        ReturnType getType();
    private:
        ReturnType type;
        std::string name;
        std::string value;
};


class ReturnValue {
    public:
        ReturnValue(ReturnType, std::string);
        ReturnValue();
        std::string getValue();
        ReturnType getType();
        void setOptionalValue(Scope*);
        Scope* getOptionalValue();
        void setOptionalType(ReturnType);
        ReturnType getOptionalType();
    private:
        ReturnType returnType;
        std::string value;
        Scope* optionalVal;
        ReturnType optionalType;
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