

#include "interpreter.h"
#include "parser.h"
#include "statement.h"
#include "util.h"
#include <math.h>

void Programm::run() {
    VarScope global_var_scope;
    VarScope local_var_scope;
    FuncScope global_func_scope;
    FuncScope local_func_scope;
    this->eval(local_var_scope, global_var_scope, local_func_scope, global_func_scope);
}

ReturnValue Programm::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    for (auto expression: this->expressions){
        ReturnValue ret_obj;
        ret_obj = expression->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
        if (ret_obj.getType() == VAR_ASSIGNMENT_TYPE) {
            global_variable_scope.push((VarInScope*) ret_obj.getOptionalValue());
        }
        else if (ret_obj.getType() == FUNC_ASSIGNMENT_TYPE) {
            global_function_scope.push((FuncInScope*) ret_obj.getOptionalValue());
        }
        else if (ret_obj.getType() == RETURN_TYPE) {
            return ReturnValue(NONE_TYPE, ""); // End Programm execution
        }
        else if (ret_obj.getType() != NONE_TYPE && ret_obj.getType() != RETURN_TYPE) {
            std::cout << ret_obj.getValue();
        }
    }
    return ReturnValue(NONE_TYPE, "");
}


ReturnValue Number::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    return ReturnValue(INT_TYPE, this->value);

}

ReturnValue String::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    return ReturnValue(STRING_TYPE, this->value);

}

ReturnValue Multiplication::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue lhs;
    lhs = this->LHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    ReturnValue rhs;
    rhs = this->RHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    std::string ret;

    if (lhs.getType() == INT_TYPE && rhs.getType() == INT_TYPE) {
        ret = multIntInt(lhs.getValue(), rhs.getValue());
        return ReturnValue(INT_TYPE, ret);
    }
    if (lhs.getType() == INT_TYPE && rhs.getType() == STRING_TYPE) {
        ret = multIntString(lhs.getValue(), rhs.getValue());
        return ReturnValue(STRING_TYPE, ret);
    }
    if (lhs.getType() == STRING_TYPE && rhs.getType() == INT_TYPE) {
        ret = multStringInt(lhs.getValue(), rhs.getValue());
        return ReturnValue(STRING_TYPE, ret);
    }
    else {
        eval_error("Multiplication does not support: " + lhs.getValue() + "*" + rhs.getValue(), 1);
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

    if (lhs.getType() == INT_TYPE && rhs.getType() == INT_TYPE) {
        ret = minusIntInt(lhs.getValue(), rhs.getValue());
        return ReturnValue(INT_TYPE, ret);
    }
    else {
        eval_error("Subtraction does not support: " + lhs.getValue() + "*" + rhs.getValue(), 5);
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

    if (lhs.getType() == INT_TYPE && rhs.getType() == INT_TYPE) {
        ret = plusIntInt(lhs.getValue(), rhs.getValue());
        return ReturnValue(INT_TYPE, ret);
    }
    if (lhs.getType() == INT_TYPE && rhs.getType() == STRING_TYPE) {
        ret = plusIntString(lhs.getValue(), rhs.getValue());
        return ReturnValue(STRING_TYPE, ret);
    }
    if (lhs.getType() == STRING_TYPE && rhs.getType() == INT_TYPE) {
        ret = plusStringInt(lhs.getValue(), rhs.getValue());
        return ReturnValue(STRING_TYPE, ret);
    }
    if (lhs.getType() == STRING_TYPE && rhs.getType() == STRING_TYPE) {
        ret = plusStringString(lhs.getValue(), rhs.getValue());
        return ReturnValue(STRING_TYPE, ret);
    }
    else {
        std::cout << std::endl;
        std::cout << lhs.getType() << " " << rhs.getType() << std::endl;
        eval_error("Addition does not support: " + lhs.getValue() + "+" + rhs.getValue(), 7);
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

ReturnValue Divide::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue lhs;
    lhs = this->LHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    ReturnValue rhs;
    rhs = this->RHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    std::string ret;

    if (lhs.getType() == INT_TYPE && rhs.getType() == INT_TYPE) {
        ret = divideIntInt(lhs.getValue(), rhs.getValue());
        return ReturnValue(INT_TYPE, ret);
    }
    else {
        eval_error("Divison does not support: " + lhs.getValue() + "/" + rhs.getValue(), 9);
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

    ret = boolToString(lhs.getValue() == rhs.getValue() && lhs.getType() == rhs.getType());

    return ReturnValue(BOOL_TYPE, ret);

}

ReturnValue Bool::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    return ReturnValue(BOOL_TYPE, this->truth);

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
    if (!(lhs.getType() == INT_TYPE)) {
        eval_error("<non int type>: " + lhs.getValue(), "<int type>", 13);
    }

    if (!(rhs.getType() == INT_TYPE)) {
        eval_error("<non int type>: " + rhs.getValue(), "<int type>", 14);
    }

    int a,b;
    try {
        a = std::stoi(lhs.getValue());
        b = std::stoi(rhs.getValue());
    }
    catch (const std::exception e) {
        eval_error("Could not convert strings to ints in less evalutation: " + lhs.getValue() + " < " + rhs.getValue(), 15);
    }

    return ReturnValue(BOOL_TYPE, boolToString(a<b));

}

ReturnValue Greater::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue lhs;
    lhs = this->LHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    ReturnValue rhs;
    rhs = this->RHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);  
    if (!(lhs.getType() == INT_TYPE)) {
        eval_error("<non int type>: " + lhs.getValue(), "<int type>", 16);
    }

    if (!(rhs.getType() == INT_TYPE)) {
        eval_error("<non int type>: " + rhs.getValue(), "<int type>", 17);
    }

    int a,b;
    try {
        a = std::stoi(lhs.getValue());
        b = std::stoi(rhs.getValue());
    }
    catch (const std::exception e) {
        eval_error("Could not convert strings to ints in greater evalutation: " + lhs.getValue() + " > " + rhs.getValue(), 18);
    }

    return ReturnValue(BOOL_TYPE, boolToString(a>b));

}

ReturnValue LessEqual::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue lhs;
    lhs = this->LHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    ReturnValue rhs;
    rhs = this->RHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);  
    if (!(lhs.getType() == INT_TYPE)) {
        eval_error("<non int type>: " + lhs.getValue(), "<int type>", 19);
    }

    if (!(rhs.getType() == INT_TYPE)) {
        eval_error("<non int type>: " + rhs.getValue(), "<int type>", 20);
    }

    int a,b;
    try {
        a = std::stoi(lhs.getValue());
        b = std::stoi(rhs.getValue());
    }
    catch (const std::exception e) {
        eval_error("Could not convert strings to ints in less equal evalutation: " + lhs.getValue() + " <= " + rhs.getValue(), 21);
    }

    return ReturnValue(BOOL_TYPE, boolToString(a<=b));

}

ReturnValue GreaterEqual::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue lhs;
    lhs = this->LHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    ReturnValue rhs;
    rhs = this->RHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);   
    if (!(lhs.getType() == INT_TYPE)) {
        eval_error("<non int type>: " + lhs.getValue(), "<int type>", 22);
    }

    if (!(rhs.getType() == INT_TYPE)) {
        eval_error("<non int type>: " + rhs.getValue(), "<int type>", 23);
    }

    int a,b;
    try {
        a = std::stoi(lhs.getValue());
        b = std::stoi(rhs.getValue());
    }
    catch (const std::exception e) {
        eval_error("Could not convert strings to ints in greater equal evalutation: " + lhs.getValue() + " >= " + rhs.getValue(), 24);
    }

    return ReturnValue(BOOL_TYPE, boolToString(a>=b));

}

ReturnValue Power::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue lhs;
    lhs = this->LHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    ReturnValue rhs;
    rhs = this->RHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);    
    if (!(lhs.getType() == INT_TYPE)) {
        eval_error("<non int type>: " + lhs.getValue(), "<int type>", 25);
    }

    if (!(rhs.getType() == INT_TYPE)) {
        eval_error("<non int type>: " + rhs.getValue(), "<int type>", 26);
    }

    int a,b;
    try {
        a = std::stoi(lhs.getValue());
        b = std::stoi(rhs.getValue());
    }
    catch (const std::exception e) {
        eval_error("Could not convert strings to ints in power evalutation: " + lhs.getValue() + " ** " + rhs.getValue(), 27);
    }

    return ReturnValue(INT_TYPE, std::to_string(ipow(a,b)));

}

ReturnValue Not::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue rhs;
    rhs = this->RHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);  
    if (!(rhs.getType() == BOOL_TYPE)) {
        eval_error("<non bool type>: " + rhs.getValue(), "<bool type>", 28);
    }

    
    bool a;
    a = stringToBool(rhs.getValue());

    return ReturnValue(BOOL_TYPE, boolToString(!a));
}

ReturnValue And::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue lhs;
    ReturnValue rhs;
    rhs = this->RHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    lhs = this->LHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    std::string ret;    
    if (!(rhs.getType() == BOOL_TYPE)) {
        eval_error("<non bool type>: " + rhs.getValue(), "<bool type>", 29);
    }
    if (!(rhs.getType() == BOOL_TYPE)) {
        eval_error("<non bool type>: " + rhs.getValue(), "<bool type>", 30);
    }

    
    bool a,b;
    a = stringToBool(lhs.getValue());
    b = stringToBool(rhs.getValue());

    return ReturnValue(BOOL_TYPE, boolToString(a && b));
}

ReturnValue Or::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue lhs;
    ReturnValue rhs;
    rhs = this->RHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    lhs = this->LHS->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    std::string ret;    
    if (!(rhs.getType() == BOOL_TYPE)) {
        eval_error("<non bool type>: " + rhs.getValue(), "<bool type>", 31);
    }
    if (!(rhs.getType() == BOOL_TYPE)) {
        eval_error("<non bool type>: " + rhs.getValue(), "<bool type>", 32);
    }

    
    bool a,b;
    a = stringToBool(lhs.getValue());
    b = stringToBool(rhs.getValue());

    return ReturnValue(BOOL_TYPE, boolToString(a || b));
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
        std::string value = ret.getValue();
        ReturnType type = ret.getType();
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
    std::string value = stat.getValue();
    ReturnType type = stat.getType();
    VarInScope* var = new VarInScope(name, value, type);
    ReturnValue ret = ReturnValue(VAR_ASSIGNMENT_TYPE, "");
    ret.setOptionalValue(var);
    return ret;
}

ReturnValue If::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue cond;
    cond = this->condition->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);

    if (stringToBool(cond.getValue())) {
        ReturnValue b;
        b = this->body->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
        if (b.getType() == RETURN_TYPE) {
            ReturnValue val = ReturnValue(RETURN_TYPE, b.getValue());
            val.setOptionalType(b.getOptionalType());
            return val;
        }
        return ReturnValue(NONE_TYPE, "");
    }
    
    return ReturnValue(NONE_TYPE, "");
}

ReturnValue While::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue cond;
    cond = this->condition->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);

    while (stringToBool(cond.getValue())) {
        ReturnValue b;
        b = this->body->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
        if (b.getType() == BREAK_TYPE) {
            return ReturnValue(NONE_TYPE, "");
        }
        else if (b.getType() == RETURN_TYPE) {
            ReturnValue val = ReturnValue(RETURN_TYPE, b.getValue());
            val.setOptionalType(b.getOptionalType());
            return val;
        }
        cond = this->condition->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    }
    return ReturnValue(NONE_TYPE, "");
}

ReturnValue For::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue in;
    ReturnValue cond;
    ReturnValue incre;
    in= this->init->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    local_variable_scope.push((VarInScope*) in.getOptionalValue());
    cond = this->condition->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);

    while (stringToBool(cond.getValue())) {

        ReturnValue b;
        b = this->body->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
        if (b.getType() == BREAK_TYPE) {
            return ReturnValue(NONE_TYPE, "");
        }
        else if (b.getType() == RETURN_TYPE) {
            ReturnValue val = ReturnValue(RETURN_TYPE, b.getValue());
            val.setOptionalType(b.getOptionalType());
            return val;
        }
        incre = this->changer->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
        local_variable_scope.push((VarInScope*) incre.getOptionalValue());
        cond = this->condition->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    }
    return ReturnValue(NONE_TYPE, "");
}

ReturnValue Return::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    ReturnValue ret;
    ret = this->return_value->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
    ReturnValue val = ReturnValue(RETURN_TYPE, ret.getValue());
    val.setOptionalType(ret.getType());
    return val;
}

ReturnValue Break::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    return ReturnValue(BREAK_TYPE, "");
}

ReturnValue FunctionDefinition::eval(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope) {

    std::string functionName = this->name;
    Block* body = this->body;
    std::vector<std::string> params = this->param_names;
    FuncInScope* func = new FuncInScope(functionName, params, body);
    ReturnValue ret = ReturnValue(FUNC_ASSIGNMENT_TYPE, "");
    ret.setOptionalValue(func);
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
            local_variable_scope.push((VarInScope*) ret_obj.getOptionalValue());
        }
        else if (ret_obj.getType() == FUNC_ASSIGNMENT_TYPE) {
            local_function_scope.push((FuncInScope*) ret_obj.getOptionalValue());
        }
        else if (ret_obj.getType() == RETURN_TYPE) {
            return ReturnValue(ret_obj.getOptionalType(), ret_obj.getValue());
        }
        else if (ret_obj.getType() != NONE_TYPE) {
            std::cout << ret_obj.getValue();
        }
    }
    return ReturnValue(NONE_TYPE, "");
}

ReturnValue evalNonFuncBlock(VarScope& local_variable_scope, VarScope& global_variable_scope, FuncScope& local_function_scope, FuncScope& global_function_scope, Block* body) {
    ReturnValue ret_obj;
    for(auto expr: body->getExpressions()) {
        ret_obj = expr->eval(local_variable_scope, global_variable_scope, local_function_scope, global_function_scope);
        if (ret_obj.getType() == VAR_ASSIGNMENT_TYPE) {
            global_variable_scope.push((VarInScope*) ret_obj.getOptionalValue());
        }
        else if (ret_obj.getType() == FUNC_ASSIGNMENT_TYPE) {
            global_function_scope.push((FuncInScope*) ret_obj.getOptionalValue());
        }
        else if (ret_obj.getType() == BREAK_TYPE) {
            return ReturnValue(BREAK_TYPE, "");
        }
        else if (ret_obj.getType() == RETURN_TYPE) {
            ReturnValue val = ReturnValue(RETURN_TYPE, ret_obj.getValue());
            val.setOptionalType(ret_obj.getOptionalType());
            return val;
        }
        else if (ret_obj.getType() != NONE_TYPE && ret_obj.getType() != RETURN_TYPE) {
            std::cout << ret_obj.getValue();
        }
    }
    return ReturnValue(NONE_TYPE, "");
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
    return ReturnValue(NONE_TYPE, "");
}


// Classes


ReturnValue::ReturnValue(ReturnType type, std::string value) {
    this->value = value;
    this->returnType = type;
}

void ReturnValue::setOptionalValue(Scope* optional) {
    this->optionalVal = optional;
}

Scope* ReturnValue::getOptionalValue() {
    return this->optionalVal;
}


std::string ReturnValue::getValue() {
    return this->value;
}

ReturnType ReturnValue::getType() {
    return this->returnType;
}

VarInScope::VarInScope(std::string name, std::string value, ReturnType type) {
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

ReturnType VarInScope::getType() {
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
            return ReturnValue(variable->getType(), variable->getValue());
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

void ReturnValue::setOptionalType(ReturnType type) {
    this->optionalType = type;
}

ReturnType ReturnValue::getOptionalType() {
    return this->optionalType;
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