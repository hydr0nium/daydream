

#include "interpreter.h"
#include "parser.h"
#include "statement.h"
#include "util.h"
#include <math.h>

void Program::run() {
    VarScope global_var_scope;
    VarScope local_var_scope;
    FuncScope global_func_scope;
    FuncScope local_func_scope;
    this->eval(local_var_scope, global_var_scope, local_func_scope, global_func_scope);
}

ReturnValue Program::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    for (auto expression: this->expressions){
        ReturnValue ret_obj;
        ret_obj = expression->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
        if (ret_obj.getType() == VAR_ASSIGNMENT_TYPE) {
            ScopeValue* scope = ((ScopeValue*) ret_obj.getValueObj());
            global_variable_scope.push((VarInScope*) scope->getValue());
        }
        else if (ret_obj.getType() == FUNC_ASSIGNMENT_TYPE) {
            ScopeValue* scope = ((ScopeValue*) ret_obj.getValueObj());
            global_function_scope.push((FuncInScope*) scope->getValue());
        }
        else if (ret_obj.getType() == RETURN_TYPE) {
            return ReturnValue(NONE_TYPE); // End Program execution
        }
        else if (ret_obj.getType() != NONE_TYPE && ret_obj.getType() == PRIMITIVE_TYPE) {
            std::cout << ((PrimitiveValue*) ret_obj.getValueObj())->getValue();
        }
    }
    return ReturnValue(NONE_TYPE);
}


ReturnValue Number::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ValueObject* val = new PrimitiveValue(this->value, INT_TYPE);
    return ReturnValue(PRIMITIVE_TYPE, val);

}

ReturnValue String::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {
    ValueObject* val = new PrimitiveValue(this->value, STRING_TYPE);
    return ReturnValue(PRIMITIVE_TYPE, val);

}

ReturnValue Multiplication::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue lhs;
    lhs = this->LHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    ReturnValue rhs;
    rhs = this->RHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    std::string ret;
    if (lhs.getType() != PRIMITIVE_TYPE || rhs.getType() != PRIMITIVE_TYPE) {
        eval_error("<primitive type>", "<non primitive type>", 182793);
    }
    PrimitiveValue* leftValue = (PrimitiveValue*) lhs.getValueObj();
    PrimitiveValue* rightValue = (PrimitiveValue*) rhs.getValueObj();
    ValueType leftType = leftValue->getType();
    ValueType rightType = rightValue->getType();
    if (leftType == INT_TYPE && rightType == INT_TYPE) {
        ret = multIntInt(leftValue->getValue(), rightValue->getValue());
        ValueObject* val = new PrimitiveValue(ret, INT_TYPE);
        return ReturnValue(PRIMITIVE_TYPE, val);
    }
    if (leftType == INT_TYPE && rightType == STRING_TYPE) {
        ret = multIntString(leftValue->getValue(), rightValue->getValue());
        ValueObject* val = new PrimitiveValue(ret, STRING_TYPE);
        return ReturnValue(PRIMITIVE_TYPE, val);
    }
    if (leftType == STRING_TYPE && rightType == INT_TYPE) {
        ret = multStringInt(leftValue->getValue(), rightValue->getValue());
        ValueObject* val = new PrimitiveValue(ret, STRING_TYPE);
        return ReturnValue(PRIMITIVE_TYPE, val);
    }
    else {
        eval_error("Multiplication does not support: " + leftValue->getValue() + "*" + rightValue->getValue(), 1);
    }
    // This can never happen because eval_error exits the programm
    return ReturnValue();

}

std::string multIntInt(std::string lhs, std::string rhs) {
    int a,b;
    try {

        a = std::stoi(lhs);
        b = std::stoi(rhs);

    }
    catch (const std::exception e) {
        eval_error("<int>*<int>", lhs + "*" + rhs, 2);
    }
    return std::to_string(a*b);
}

std::string multIntString(std::string integer, std::string str) {
    int count;
    std::string ret = "";
    try {
        count = std::stoi(integer);
    }
    catch (const std::exception e) {
        eval_error("<int>*<string>", integer + "*" + str, 3);
    }
    for (int i = 0; i < count; i++) {
        ret += str;
    }
    return ret;

}

std::string multStringInt(std::string str, std::string integer) {
    int count;
    std::string ret = "";
    try {
        count = std::stoi(integer);
    }
    catch (const std::exception e) {
        eval_error("<string>*<int>", str + "*" + integer, 4);
    }
    for (int i = 0; i < count; i++) {
        ret += str;
    }
    return ret;

}

ReturnValue Minus::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue lhs;
    lhs = this->LHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    ReturnValue rhs;
    rhs = this->RHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    std::string ret;

    if (lhs.getType() != PRIMITIVE_TYPE || rhs.getType() != PRIMITIVE_TYPE) {
        eval_error("<primitive type>", "<non primitive type>", 182794);
    }
    PrimitiveValue* leftValue = (PrimitiveValue*) lhs.getValueObj();
    PrimitiveValue* rightValue = (PrimitiveValue*) rhs.getValueObj();
    ValueType leftType = leftValue->getType();
    ValueType rightType = rightValue->getType();

    if (leftType == INT_TYPE && rightType == INT_TYPE) {
        ret = minusIntInt(leftValue->getValue(), rightValue->getValue());
        ValueObject* val = new PrimitiveValue(ret, INT_TYPE);
        return ReturnValue(PRIMITIVE_TYPE, val);
    }
    else {
        eval_error("Subtraction does not support: " + leftValue->getValue() + "*" + rightValue->getValue(), 5);
    }

    // This can never happen because eval_error exits the programm
    return ReturnValue();

}

std::string minusIntInt(std::string lhs, std::string rhs) {
    int a,b;
    try {
        a = std::stoi(lhs);
        b = std::stoi(rhs);
    }
    catch (const std::exception e) {
        eval_error("<int>-<int>", lhs + "-" + "rhs", 6);
    }
    return std::to_string(a-b);
}

ReturnValue Plus::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue lhs;
    lhs = this->LHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    ReturnValue rhs;
    rhs = this->RHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    std::string ret;

    if (lhs.getType() != PRIMITIVE_TYPE || rhs.getType() != PRIMITIVE_TYPE) {
        eval_error("<primitive type>", "<non primitive type>", 182795);
    }
    PrimitiveValue* leftValue = (PrimitiveValue*) lhs.getValueObj();
    PrimitiveValue* rightValue = (PrimitiveValue*) rhs.getValueObj();
    ValueType leftType = leftValue->getType();
    ValueType rightType = rightValue->getType();

    if (leftType == INT_TYPE && rightType == INT_TYPE) {
        ret = plusIntInt(leftValue->getValue(), rightValue->getValue());
        ValueObject* val = new PrimitiveValue(ret, INT_TYPE);
        return ReturnValue(PRIMITIVE_TYPE, val);
    }
    if (leftType == INT_TYPE && rightType == STRING_TYPE) {
        ret = plusIntString(leftValue->getValue(), rightValue->getValue());
        ValueObject* val = new PrimitiveValue(ret, STRING_TYPE);
        return ReturnValue(PRIMITIVE_TYPE, val);
    }
    if (leftType == STRING_TYPE && rightType == INT_TYPE) {
        ret = plusStringInt(leftValue->getValue(), rightValue->getValue());
        ValueObject* val = new PrimitiveValue(ret, STRING_TYPE);
        return ReturnValue(PRIMITIVE_TYPE, val);
    }
    if (leftType == STRING_TYPE && rightType == STRING_TYPE) {
        ret = plusStringString(leftValue->getValue(), rightValue->getValue());
        ValueObject* val = new PrimitiveValue(ret, STRING_TYPE);
        return ReturnValue(PRIMITIVE_TYPE, val);
    }
    if (leftType == BOOL_TYPE && rightType == STRING_TYPE) {
        ret = plusBoolString(leftValue->getValue(), rightValue->getValue());
        ValueObject* val = new PrimitiveValue(ret, STRING_TYPE);
        return ReturnValue(PRIMITIVE_TYPE, val);
    }
    if (leftType == STRING_TYPE && rightType == BOOL_TYPE) {
        ret = plusStringBool(leftValue->getValue(), rightValue->getValue());
        ValueObject* val = new PrimitiveValue(ret, STRING_TYPE);
        return ReturnValue(PRIMITIVE_TYPE, val);
    }
    else {
        eval_error("Addition does not support: " + leftValue->getValue() + "+" + rightValue->getValue(), 7);
    }

    // This can never happen because eval_error exits the programm
    return ReturnValue();

}

std::string plusIntInt(std::string lhs, std::string rhs) {
    int a,b;
    try {
        a = std::stoi(lhs);
        b = std::stoi(rhs);
    }
    catch (const std::exception e) {
        eval_error("<int>+<int>", lhs + "+" + rhs, 8);
    }
    return std::to_string(a+b);
}

std::string plusIntString(std::string lhs, std::string rhs) {

    return lhs + rhs;
}

std::string plusStringInt(std::string lhs, std::string rhs) {
    return lhs + rhs;
}

std::string plusStringString(std::string lhs, std::string rhs) {
    return lhs + rhs;
}

std::string plusBoolString(std::string lhs, std::string rhs) {
    return lhs + rhs;
}

std::string plusStringBool(std::string lhs, std::string rhs) {
    return lhs + rhs;
}

ReturnValue Divide::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue lhs;
    lhs = this->LHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    ReturnValue rhs;
    rhs = this->RHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    std::string ret;
    if (lhs.getType() != PRIMITIVE_TYPE || rhs.getType() != PRIMITIVE_TYPE) {
        eval_error("<primitive type>", "<non primitive type>", 182796);
    }
    PrimitiveValue* leftValue = (PrimitiveValue*) lhs.getValueObj();
    PrimitiveValue* rightValue = (PrimitiveValue*) rhs.getValueObj();
    ValueType leftType = leftValue->getType();
    ValueType rightType = rightValue->getType();

    if (leftType == INT_TYPE && rightType == INT_TYPE) {
        ret = divideIntInt(leftValue->getValue(), rightValue->getValue());
        ValueObject* val = new PrimitiveValue(ret, INT_TYPE);
        return ReturnValue(PRIMITIVE_TYPE, val);
    }
    else {
        eval_error("Divison does not support: " + leftValue->getValue() + "/" + rightValue->getValue(), 9);
    }

    // This can never happen because eval_error exits the programm
    return ReturnValue();

}

std::string divideIntInt(std::string lhs, std::string rhs) {
    int a,b;
    try {
        a = std::stoi(lhs);
        b = std::stoi(rhs);
    }
    catch (const std::exception e) {
        eval_error("<int>/<int>", lhs + "/" + "rhs", 10);
    }
    return std::to_string(a/b);
}

ReturnValue Equality::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue lhs;
    lhs = this->LHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    ReturnValue rhs;
    rhs = this->RHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    std::string ret;
    if (lhs.getType() != PRIMITIVE_TYPE || rhs.getType() != PRIMITIVE_TYPE) {
        eval_error("<primitive type>", "<non primitive type>", 182797);
    }
    PrimitiveValue* leftValue = (PrimitiveValue*) lhs.getValueObj();
    PrimitiveValue* rightValue = (PrimitiveValue*) rhs.getValueObj();
    ValueType leftType = leftValue->getType();
    ValueType rightType = rightValue->getType();

    ret = boolToString(leftValue->getValue() == rightValue->getValue() && leftType==rightType);
    ValueObject* val = new PrimitiveValue(ret, BOOL_TYPE);
    return ReturnValue(PRIMITIVE_TYPE, val);

}

ReturnValue Bool::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ValueObject* val = new PrimitiveValue(this->truth, BOOL_TYPE);
    return ReturnValue(PRIMITIVE_TYPE, val);

}


ReturnValue LParen::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    eval_error("Can not eval left parenthesis.", 11);

    return ReturnValue();

}

ReturnValue RParen::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    eval_error("Can not eval left parenthesis.", 12);

    return ReturnValue();

}

ReturnValue Less::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue lhs;
    lhs = this->LHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    ReturnValue rhs;
    rhs = this->RHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);  

    if (lhs.getType() != PRIMITIVE_TYPE || rhs.getType() != PRIMITIVE_TYPE) {
        eval_error("<primitive type>", "<non primitive type>", 182798);
    }
    PrimitiveValue* leftValue = (PrimitiveValue*) lhs.getValueObj();
    PrimitiveValue* rightValue = (PrimitiveValue*) rhs.getValueObj();
    ValueType leftType = leftValue->getType();
    ValueType rightType = rightValue->getType();
    
      
    if (!(leftValue->getType() == INT_TYPE)) {
        eval_error("<int type>", "<non int type> (Value:"  + leftValue->getValue() + ")", 13);
    }

    if (!(rightValue->getType() == INT_TYPE)) {
        eval_error("<int type>", "<non int type> (Value:"  + rightValue->getValue() + ")", 14);
    }

    int a,b;
    try {
        a = std::stoi(leftValue->getValue());
        b = std::stoi(rightValue->getValue());
    }
    catch (const std::exception e) {
        eval_error("Could not convert strings to ints in less evalutation: " + leftValue->getValue() + " < " + rightValue->getValue(), 15);
    }

    ValueObject* val = new PrimitiveValue(boolToString(a<b), BOOL_TYPE);
    return ReturnValue(PRIMITIVE_TYPE, val);

}

ReturnValue Greater::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue lhs;
    lhs = this->LHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    ReturnValue rhs;
    rhs = this->RHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope); 

    if (lhs.getType() != PRIMITIVE_TYPE || rhs.getType() != PRIMITIVE_TYPE) {
        eval_error("<primitive type>", "<non primitive type>", 182799);
    }
    PrimitiveValue* leftValue = (PrimitiveValue*) lhs.getValueObj();
    PrimitiveValue* rightValue = (PrimitiveValue*) rhs.getValueObj();
    ValueType leftType = leftValue->getType();
    ValueType rightType = rightValue->getType();

    if (!(leftValue->getType() == INT_TYPE)) {
        eval_error("<int type>", "<non int type> (Value:"  + leftValue->getValue() + ")", 16);
    }

    if (!(rightValue->getType() == INT_TYPE)) {
        eval_error("<int type>", "<non int type> (Value:"  + rightValue->getValue() + ")", 17);
    }

    int a,b;
    try {
        a = std::stoi(leftValue->getValue());
        b = std::stoi(rightValue->getValue());
    }
    catch (const std::exception e) {
        eval_error("Could not convert strings to ints in greater evalutation: " + leftValue->getValue() + " > " + rightValue->getValue(), 18);
    }

    ValueObject* val = new PrimitiveValue(boolToString(a>b), BOOL_TYPE);
    return ReturnValue(PRIMITIVE_TYPE, val);

}

ReturnValue LessEqual::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue lhs;
    lhs = this->LHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    ReturnValue rhs;
    rhs = this->RHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope); 

    if (lhs.getType() != PRIMITIVE_TYPE || rhs.getType() != PRIMITIVE_TYPE) {
        eval_error("<primitive type>", "<non primitive type>", 182800);
    }
    PrimitiveValue* leftValue = (PrimitiveValue*) lhs.getValueObj();
    PrimitiveValue* rightValue = (PrimitiveValue*) rhs.getValueObj();
    ValueType leftType = leftValue->getType();
    ValueType rightType = rightValue->getType();

    if (!(leftValue->getType() == INT_TYPE)) {
        eval_error("<int type>", "<non int type> (Value:"  + leftValue->getValue() + ")", 19);
    }

    if (!(rightValue->getType() == INT_TYPE)) {
        eval_error("<int type>", "<non int type> (Value:"  + rightValue->getValue() + ")", 20);
    }

    int a,b;
    try {
        a = std::stoi(leftValue->getValue());
        b = std::stoi(rightValue->getValue());
    }
    catch (const std::exception e) {
        eval_error("Could not convert strings to ints in less equal evalutation: " + leftValue->getValue() + " <= " + rightValue->getValue(), 21);
    }

    ValueObject* val = new PrimitiveValue(boolToString(a<=b), BOOL_TYPE);
    return ReturnValue(PRIMITIVE_TYPE, val);

}

ReturnValue GreaterEqual::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue lhs;
    lhs = this->LHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    ReturnValue rhs;
    rhs = this->RHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);   
    if (lhs.getType() != PRIMITIVE_TYPE || rhs.getType() != PRIMITIVE_TYPE) {
        eval_error("<primitive type>", "<non primitive type>", 182801);
    }
    PrimitiveValue* leftValue = (PrimitiveValue*) lhs.getValueObj();
    PrimitiveValue* rightValue = (PrimitiveValue*) rhs.getValueObj();
    ValueType leftType = leftValue->getType();
    ValueType rightType = rightValue->getType();

    if (!(leftValue->getType() == INT_TYPE)) {
        eval_error("<int type>", "<non int type> (Value:"  + leftValue->getValue() + ")", 22);
    }

    if (!(rightValue->getType() == INT_TYPE)) {
        eval_error("<int type>", "<non int type> (Value:"  + rightValue->getValue() + ")", 23);
    }
    

    int a,b;
    try {
        a = std::stoi(leftValue->getValue());
        b = std::stoi(rightValue->getValue());
    }
    catch (const std::exception e) {
        eval_error("Could not convert strings to ints in greater equal evalutation: " + leftValue->getValue() + " >= " + rightValue->getValue(), 24);
    }

    ValueObject* val = new PrimitiveValue(boolToString(a>=b), BOOL_TYPE);
    return ReturnValue(PRIMITIVE_TYPE, val);

}

ReturnValue Power::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue lhs;
    lhs = this->LHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    ReturnValue rhs;
    rhs = this->RHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);  
    if (lhs.getType() != PRIMITIVE_TYPE || rhs.getType() != PRIMITIVE_TYPE) {
        eval_error("<primitive type>", "<non primitive type>", 182802);
    }
    PrimitiveValue* leftValue = (PrimitiveValue*) lhs.getValueObj();
    PrimitiveValue* rightValue = (PrimitiveValue*) rhs.getValueObj();
    ValueType leftType = leftValue->getType();
    ValueType rightType = rightValue->getType();

    if (!(leftValue->getType() == INT_TYPE)) {
        eval_error("<int type>", "<non int type> (Value:"  + leftValue->getValue() + ")", 25);
    }

    if (!(rightValue->getType() == INT_TYPE)) {
        eval_error("<int type>", "<non int type> (Value:"  + rightValue->getValue() + ")", 26);
    }  
    

    int a,b;
    try {
        a = std::stoi(leftValue->getValue());
        b = std::stoi(rightValue->getValue());
    }
    catch (const std::exception e) {
        eval_error("Could not convert strings to ints in power evalutation: " + leftValue->getValue() + " ** " + rightValue->getValue(), 27);
    }

    ValueObject* val = new PrimitiveValue(std::to_string(ipow(a,b)), INT_TYPE);
    return ReturnValue(PRIMITIVE_TYPE, val);

}

ReturnValue Not::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue rhs;
    rhs = this->RHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);  
    if (rhs.getType() != PRIMITIVE_TYPE) {
        eval_error("<primitive type>", "<non primitive type>", 182803);
    }
    PrimitiveValue* rightValue = (PrimitiveValue*) rhs.getValueObj();
    ValueType rightType = rightValue->getType();
    if (!(rightValue->getType() == BOOL_TYPE)) {
        eval_error("<bool type>", "<non bool type> (Value:"  + rightValue->getValue() + ")", 27);
    }  

    
    bool a;
    a = stringToBool(rightValue->getValue());


    ValueObject* val = new PrimitiveValue(boolToString(!a), BOOL_TYPE);
    return ReturnValue(PRIMITIVE_TYPE, val);
}

ReturnValue And::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue lhs;
    ReturnValue rhs;
    rhs = this->RHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    lhs = this->LHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);   

    if (lhs.getType() != PRIMITIVE_TYPE || rhs.getType() != PRIMITIVE_TYPE) {
        eval_error("<primitive type>", "<non primitive type>", 182804);
    }
    PrimitiveValue* leftValue = (PrimitiveValue*) lhs.getValueObj();
    PrimitiveValue* rightValue = (PrimitiveValue*) rhs.getValueObj();
    ValueType leftType = leftValue->getType();
    ValueType rightType = rightValue->getType();

    if (!(leftValue->getType() == BOOL_TYPE)) {
        eval_error("<bool type>", "<non bool type> (Value:"  + leftValue->getValue() + ")", 28);
    }

    if (!(rightValue->getType() == BOOL_TYPE)) {
        eval_error("<bool type>", "<non bool type> (Value:"  + rightValue->getValue() + ")", 29);
    }  

    
    bool a,b;
    a = stringToBool(leftValue->getValue());
    b = stringToBool(rightValue->getValue());


    ValueObject* val = new PrimitiveValue(boolToString(a && b), BOOL_TYPE);
    return ReturnValue(PRIMITIVE_TYPE, val);
}

ReturnValue Or::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue lhs;
    ReturnValue rhs;
    rhs = this->RHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    lhs = this->LHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);   

    if (lhs.getType() != PRIMITIVE_TYPE || rhs.getType() != PRIMITIVE_TYPE) {
        eval_error("<primitive type>", "<non primitive type>", 182805);
    }
    PrimitiveValue* leftValue = (PrimitiveValue*) lhs.getValueObj();
    PrimitiveValue* rightValue = (PrimitiveValue*) rhs.getValueObj();
    ValueType leftType = leftValue->getType();
    ValueType rightType = rightValue->getType();

    if (!(leftValue->getType() == BOOL_TYPE)) {
        eval_error("<bool type>", "<non bool type> (Value:"  + leftValue->getValue() + ")", 30);
    }

    if (!(rightValue->getType() == BOOL_TYPE)) {
        eval_error("<bool type>", "<non bool type> (Value:"  + rightValue->getValue() + ")", 31);
    }  
    

    
    bool a,b;
    a = stringToBool(leftValue->getValue());
    b = stringToBool(rightValue->getValue());

    ValueObject* val = new PrimitiveValue(boolToString(a || b), BOOL_TYPE);
    return ReturnValue(PRIMITIVE_TYPE, val);
}

ReturnValue Variable::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    std::string name = this->name;
    if (local_variable_scope.isInScope(name)) {
        return local_variable_scope.getValueByName(name);
    }
    else if (global_variable_scope.isInScope(name)) {
        return global_variable_scope.getValueByName(name);
    }
    else {
        eval_error("Can not find variable with name: " + name, 33);
    }

    // This can never happen because eval error quits execution
    return ReturnValue();
    
}

ReturnValue FunctionCall::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    std::string functionName = this->functionName;
    int param_count = this->params.params.size();
    FuncInScope* func;
    if (local_function_scope.isInScope(functionName, param_count)) {
        func = local_function_scope.getFuncByNameAndParamCount(functionName, param_count);
    }
    else if (global_function_scope.isInScope(functionName, param_count)) {
        func = global_function_scope.getFuncByNameAndParamCount(functionName, param_count);
    }
    else {
        eval_error("Can not find function with name: " + functionName + ", and parameter count of " + std::to_string(param_count), 34);
    }
    std::vector<std::string> param_names = func->getParams();
    VarScope new_local_variable_scope;
    FuncScope new_local_function_scope;

    int i = 0;
    for (auto parameter: params.params) {
        ReturnValue ret;
        ret = parameter->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
        std::string var_name = param_names.at(i);
        if (ret.getType() != PRIMITIVE_TYPE) {
            eval_error("<primitive type>", "<non primitive type>", 9001);
        }
        std::string value = ((PrimitiveValue*) ret.getValueObj())->getValue();
        ValueType type = ((PrimitiveValue*) ret.getValueObj())->getType();
        VarInScope* var = new VarInScope(var_name, value, type);
        new_local_variable_scope.push(var);
        i++;
    }
    ReturnValue ret_func;
    ret_func = func->getBody()->eval(new_local_variable_scope, global_variable_scope, new_local_function_scope, global_function_scope);
    return ret_func;
}


ReturnValue VariableDeclaration::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    std::string name = this->var_name;
    ReturnValue stat;
    stat = this->assigned_stm->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    if (stat.getType() != PRIMITIVE_TYPE) {
            eval_error("<primitive type>", "<non primitive type>", 9002);
    }
    PrimitiveValue* val = (PrimitiveValue*) stat.getValueObj();
    std::string value = val->getValue();
    ValueType type = val->getType();
    VarInScope* var = new VarInScope(name, value, type);
    ValueObject* scp = new ScopeValue(var);
    ReturnValue ret = ReturnValue(VAR_ASSIGNMENT_TYPE, scp);
    return ret;
}

ReturnValue Dot::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    if(RHS->getType() != FUNC_TYPE) {
        eval_error("<function>", "no function type", 9450);
    }

    ReturnValue lhs = LHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    FunctionCall* RHS = (FunctionCall*) this->RHS;
    if (lhs.getType() == PRIMITIVE_TYPE) {
        
        PrimitiveValue* pval = (PrimitiveValue*) lhs.getValueObj();
        std::string mangle = "__" + enum_to_string(pval->getType()) + "__";
        RHS->functionName = mangle + RHS->functionName;
        Statement* stm = convertReturnToExpression(lhs);
        RHS->params.addParamAtFront(stm);
        ReturnValue ret = RHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
        delete stm;
        return ret;
    }
    else {
        eval_error("This is a temporary message", 238794);
    }
    return ReturnValue();
}

ReturnValue If::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue cond;
    cond = this->condition->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);

    if (cond.getType() != PRIMITIVE_TYPE) {
        eval_error("<primitive type>", "<non primitive type>", 9003);
    }
    PrimitiveValue* value = (PrimitiveValue*) cond.getValueObj();
    if (stringToBool(value->getValue())) {
        ReturnValue b;
        b = this->body->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
        if (b.getType() == BREAK_TYPE) {
            return ReturnValue(BREAK_TYPE);
        }
        if (b.getType() == RETURN_TYPE) {
            PrimitiveValue* b_value = (PrimitiveValue*) b.getValueObj();
            ValueObject* val = new PrimitiveValue(b_value->getValue(), b_value->getType());
            ReturnValue ret = ReturnValue(RETURN_TYPE, val);
            return ret;
        }
        return ReturnValue(NONE_TYPE);
    }
    
    return ReturnValue(NONE_TYPE);
}

ReturnValue While::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue cond;
    cond = this->condition->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    if (cond.getType() != PRIMITIVE_TYPE) {
        eval_error("<primitive type>", "<non primitive type>", 9005);
    }
    PrimitiveValue* value = (PrimitiveValue*) cond.getValueObj();
    while (stringToBool(value->getValue())) {
        ReturnValue b;
        b = this->body->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
        if (b.getType() == BREAK_TYPE) {
            return ReturnValue(NONE_TYPE);
        }
        else if (b.getType() == RETURN_TYPE) {
            PrimitiveValue* b_value = (PrimitiveValue*) b.getValueObj();
            ValueObject* val = new PrimitiveValue(b_value->getValue(), b_value->getType());
            ReturnValue ret = ReturnValue(RETURN_TYPE, val);
            return ret;
        }
        cond = this->condition->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
        if (cond.getType() != PRIMITIVE_TYPE) {
            eval_error("<primitive type>", "<non primitive type>", 9007);
        }
        value = (PrimitiveValue*) cond.getValueObj();
    }
    return ReturnValue(NONE_TYPE);
}

ReturnValue For::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue in;
    ReturnValue cond;
    ReturnValue incre;
    in= this->init->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    if (in.getType() != VAR_ASSIGNMENT_TYPE) {
        eval_error("Variable Assignment in For Expression", 9008);
    }
    local_variable_scope.push((VarInScope*)((ScopeValue*) in.getValueObj())->getValue());
    cond = this->condition->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);

    if (cond.getType() != PRIMITIVE_TYPE) {
        eval_error("<primitive type>", "<non primitive type>", 9009);
    }
    PrimitiveValue* value = (PrimitiveValue*) cond.getValueObj();
    while (stringToBool(value->getValue())) {

        ReturnValue b;
        b = this->body->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
        if (b.getType() == BREAK_TYPE) {
            return ReturnValue(NONE_TYPE);
        }
        else if (b.getType() == RETURN_TYPE) {
            PrimitiveValue* b_value = (PrimitiveValue*) b.getValueObj();
            ValueObject* val = new PrimitiveValue(b_value->getValue(), b_value->getType());
            ReturnValue ret = ReturnValue(RETURN_TYPE, val);
            return ret;
        }
        incre = this->changer->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
        if (in.getType() != VAR_ASSIGNMENT_TYPE) {
            eval_error("Non Variable Assignment in For Expression", 9010);
        }
        local_variable_scope.push((VarInScope*) ((ScopeValue*) incre.getValueObj())->getValue());
        cond = this->condition->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
        if (cond.getType() != PRIMITIVE_TYPE) {
            eval_error("<primitive type>", "<non primitive type>", 9011);
        }
        value = (PrimitiveValue*) cond.getValueObj();
    }
    return ReturnValue(NONE_TYPE);
}

ReturnValue Return::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue ret;
    ret = this->return_value->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    if (ret.getType() != PRIMITIVE_TYPE) {
        eval_error("<primitive type>", "<non primitive type>", 9012);
    }
    PrimitiveValue* val = (PrimitiveValue*) ret.getValueObj();
    ValueObject* value = new PrimitiveValue(val->getValue(), val->getType());
    ret = ReturnValue(RETURN_TYPE, value);
    return ret;
}

ReturnValue Break::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {
    return ReturnValue(BREAK_TYPE);
}

ReturnValue FunctionDefinition::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    std::string functionName = this->name;
    Block* body = this->body;
    std::vector<std::string> params = this->param_names;
    FuncInScope* func = new FuncInScope(functionName, params, body);
    ValueObject* val = new ScopeValue(func);
    ReturnValue ret = ReturnValue(FUNC_ASSIGNMENT_TYPE, val);
    return ret;
}

ReturnValue Block::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    if (this->type == FUNC) {
        return evalFuncBlock(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope, this);
    }
    return evalNonFuncBlock(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope, this);
}

ReturnValue evalFuncBlock(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope, Block* body) {
    ReturnValue ret_obj;
    for(auto expr: body->getExpressions()) {
        ret_obj = expr->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
        if (ret_obj.getType() == VAR_ASSIGNMENT_TYPE) {
            local_variable_scope.push((VarInScope*) ((ScopeValue*)ret_obj.getValueObj())->getValue());
        }
        else if (ret_obj.getType() == FUNC_ASSIGNMENT_TYPE) {
            local_function_scope.push((FuncInScope*) ((ScopeValue*)ret_obj.getValueObj())->getValue());
        }
        else if (ret_obj.getType() == RETURN_TYPE) {
            // Change this later such that I know if the object is primitive or an object
            return ReturnValue(PRIMITIVE_TYPE, ret_obj.getValueObj());
        }
        else if (ret_obj.getType() != NONE_TYPE && ret_obj.getType() == PRIMITIVE_TYPE) {
            std::cout << ((PrimitiveValue*) ret_obj.getValueObj())->getValue();
        }
    }
    return ReturnValue(NONE_TYPE);
}

ReturnValue evalNonFuncBlock(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope, Block* body) {
    ReturnValue ret_obj;
    for(auto expr: body->getExpressions()) {
        ret_obj = expr->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
        if (ret_obj.getType() == VAR_ASSIGNMENT_TYPE) {
            global_variable_scope.push((VarInScope*) ((ScopeValue*)ret_obj.getValueObj())->getValue());
        }
        else if (ret_obj.getType() == FUNC_ASSIGNMENT_TYPE) {
            global_function_scope.push((FuncInScope*) ((ScopeValue*)ret_obj.getValueObj())->getValue());
        }
        else if (ret_obj.getType() == BREAK_TYPE) {
            return ReturnValue(BREAK_TYPE);
        }
        else if (ret_obj.getType() == RETURN_TYPE) {

            return ReturnValue(RETURN_TYPE, ret_obj.getValueObj());

        }
        else if (ret_obj.getType() != NONE_TYPE && ret_obj.getType() == PRIMITIVE_TYPE) {
            std::cout << ((PrimitiveValue*) ret_obj.getValueObj())->getValue();
        }
    }
    return ReturnValue(NONE_TYPE);
}

ReturnValue Debug::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {


    std::cout << "------------------" << "DEBUG-INFO" << "------------------\n";
    std::cout << "Local Variables:\n";
    for(auto var: local_variable_scope.getScope()) {
        std::cout << var->getName() + " (" + enum_to_string(var->getType()) + "): " + var->getValue() + "\n";
    }
    std::cout << "Global Variables:\n";
    for(auto var: global_variable_scope.getScope()) {
        std::cout << var->getName() + " (" + enum_to_string(var->getType()) + "): " + var->getValue() + "\n";
    }
    std::cout << "------------------" << "DEBUG--END" << "------------------\n";
    return ReturnValue(NONE_TYPE);
}


// Classes


ReturnValue::ReturnValue(ReturnType type, ValueObject* value) {
    this->valueObj = value;
    this->returnType = type;
}

ReturnValue::ReturnValue(ReturnType type) {
    this->valueObj = NULL;
    this->returnType = type;
}

ValueObject* ReturnValue::getValueObj() {
    return this->valueObj;
}

ReturnType ReturnValue::getType() {
    return this->returnType;
}

PrimitiveValue::PrimitiveValue(std::string value, ValueType type) {
    this->value = value;
    this->type = type;
}

ValueType PrimitiveValue::getType() {
    return this->type;
}

std::string PrimitiveValue::getValue() {
    return this->value;
}

VarInScope::VarInScope(std::string name, std::string value, ValueType type) {
    this->name = name;
    this->value = value;
    this->type = type;
}

FuncInScope::FuncInScope(std::string name, std::vector<std::string> param_names, Block* body) {
    this->name = name;
    this->param_names = param_names;
    this->body = body;
}

FuncScope::FuncScope(std::vector<FuncInScope*> scope) {
    this->scope = scope;
}

VarScope::VarScope(std::vector<VarInScope*> scope) {
    this->scope = scope;
}

std::string VarInScope::getName() {
    return this->name;
}

std::string VarInScope::getValue() {
    return this->value;
}

ValueType VarInScope::getType() {
    return this->type;
}

bool VarScope::isInScope(std::string name) {
    for (auto variable: this->scope) {
        if (variable->getName() == name) {
            return true;
        }
    }
    return false;
}

ReturnValue VarScope::getValueByName(std::string name) {
    for (auto variable: this->scope) {
        if (variable->getName() == name) {
            ValueObject* val = new PrimitiveValue(variable->getValue(), variable->getType());
            return ReturnValue(PRIMITIVE_TYPE, val);
        }
    }
    eval_error("Can not find variable with name: " + name, 35);
    return ReturnValue();
}

std::string FuncInScope::getName() {
    return this->name;
}
std::vector<std::string> FuncInScope::getParams() {
    return this->param_names;
}
Block* FuncInScope::getBody() {
    return this->body;
}

bool FuncScope::isInScope(std::string name, int number_of_params) {
    for (auto func: this->scope) {
        if(func->getName() == name && func->getParams().size() == number_of_params) {
            return true;
        }
    }
    return false;
}

FuncInScope* FuncScope::getFuncByNameAndParamCount(std::string name, int param_count) {
    for (auto func: this->scope) {
        if(func->getName() == name && func->getParams().size() == param_count) {
            return func;
        }
    }
    eval_error("Can not find function with name: " + name + ", and parameter count of " + std::to_string(param_count), 36);
    return NULL;
}

void VarScope::push(VarInScope* variable) {

    int i = 0;
    int found = -1;
    for (auto var: this->scope) {
        if (variable->getName() == var->getName()) {
            found = i;
        }
        i++;
    }
    if (found != -1) {
        scope[found] = variable;
    }
    else {
        this->scope.push_back(variable);
    }
}


void FuncScope::push(FuncInScope* function) {

    int i = 0;
    int found = -1;
    for (auto func: this->scope) {
        if(func->getName() == function->getName() && func->getParams().size() == function->getParams().size()) {
            found = i;
        }
        i++;
    }
    if (found != -1) {
        scope[found] = function;
    }
    else {
        this->scope.push_back(function);
    }
}

FuncScope::FuncScope() {};
VarScope::VarScope() {};
ReturnValue::ReturnValue() {};

std::vector<VarInScope*> VarScope::getScope() {
    return this->scope;
}

std::vector<FuncInScope*> FuncScope::getScope() {
    return this->scope;
}

ScopeValue::ScopeValue(Scope* scope) {
    this->scope = scope;
}

Scope* ScopeValue::getValue() {
    return this->scope;
}