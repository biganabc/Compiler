//
// Created by HP on 2021/11/13.
//

#ifndef COMPILER_CODER_H
#define COMPILER_CODER_H

#include "Stm.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <tuple>
#include <sstream>

class Memory {

public :

    int retValue = 4;       // 有没有返回值都有这块地址
    int retAddr = 4;        // 默认有返回地址
    int prev_abp = 4;       // 默认有上层abp
    vector<tuple<int, int> > paras;  // 各参数占据的地址大小
    vector<tuple<int, int> > temps;  // 各临时变量占据的地址大小

    int getRetValueOffset() {
        return 0;
    }

    int getRetAddrOffset() {
        return getRetValueOffset() + retValue;
    }

    int getPrevApbOffset() {
        return getRetAddrOffset() + retAddr;
    }

    int getParaOffset(int paraIndex) {
        int offset = getPrevApbOffset() + prev_abp;
        for (auto para:paras) {
            if (get<0>(para) != paraIndex) {
                offset += 4;    // 参数都是数，即使是数组，也是地址，所以占用的空间都是4
            } else {
                return offset;
            }
        }
        return -1;  // 正常不会到这里。
    }

    int getTempOffset(int tempIndex) {
        int sum = getPrevApbOffset() + prev_abp + (int) paras.size() * 4;
        for (auto temp :temps) {
            if (get<0>(temp) != tempIndex) {
                sum += get<1>(temp);  // 临时变量可能是数，也可能是数组
            } else {
                return sum;
            }
        }
        return -1;  // 正常不会到这里
    }

    int getAllOffSet() {  // 当前活动记录的总大小
        int sum = retValue + retAddr + prev_abp + (int) paras.size() * 4;
        for (auto par:temps) {
            sum += get<1>(par);
        }
        return sum;
    }
};

class OneCode {
public:
    string type = "";
    int register1 = -1;
    int register2 = -1;
    int register3 = -1;
    int address = -1;
    string information = "";

    OneCode(string type_, int register1, int register2, int register3, int address, string information) {
        this->type = type_;
        this->register1 = register1;
        this->register2 = register2;
        this->register3 = register3;
        this->address = address;
        this->information = information;
    }

    string toStr() {
        string str;
        if (type == "lw") {
            str = lw();
        } else if (type == "sw") {
            str = sw();
        } else if (type == "sll") {
            str = sll();
        } else if (type == "add") {
            str = add();
        } else if (type == "addi") {
            str = addi();
        } else if (type == "mul") {
            str = mul();
        } else if (type == "sub") {
            str = sub();
        } else if (type == "subi") {
            str = subi();
        } else if (type == "div") {
            str = div();
        } else if (type == "mfhi") {
            str = mfhi();
        } else if (type == "seq") {
            str = seq();
        } else if (type == "sne") {
            str = sne();
        } else if (type == "sgt") {
            str = sgt();
        } else if (type == "sge") {
            str = sge();
        } else if (type == "slt") {
            str = slt();
        } else if (type == "sle") {
            str = sle();
        } else if (type == "li") {
            str = li();
        } else if (type == "syscall") {
            str = syscall();
        } else if (type == "la") {
            str = la();
        } else if (type == "label") {
            str = label();
        } else if (type == "j") {
            str = j();
        } else if (type == "bne") {
            str = bne();
        } else if (type == "beq") {
            str = beq();
        } else if (type == "jal") {
            str = jal();
        } else if (type == "jr") {
            str = jr();
        } else if (type == "move") {
            str = move();
        }
        if (str == "") {
            cout << "不能识别的指令" << type;
        }
        return str; // 正常不会到这里
    }

    string lw() {
        if (information == "offset") {  // lw $1 100($2)
            stringstream pool;
            pool << "lw $" << register1 << " " << address << "($" << register2 << ")";
            return pool.str();
        } else if (information == "") {


        }
        return ""; // 正常情况不会遇到这里
    }

    string sw() {
        if (information == "offset") {
            stringstream pool;
            pool << "sw $" << register1 << " " << address << "($" << register2 << ")";
            return pool.str();
        } else if (information == "") {

        }
        return "";
    }

    string sll() {
        if (information == "") {
            stringstream pool;
            pool << "sll " << "$" << register1 << " " << "$" << register2 << " " << address;
            return pool.str();
        }
        return "";
    }

    string add() {
        if (information == "") {
            stringstream pool;
            pool << "addu " << "$" << register1 << " " << "$" << register2 << " " << "$" << register3;
            return pool.str();
        }
        return "";
    }

    string addi() {
        if (information == "") {
            stringstream pool;
            pool << "addiu " << "$" << register1 << " " << "$" << register2 << " " << address;
            return pool.str();
        }
        return "";
    }

    string mul() {
        if (information == "") {
            stringstream pool;
            pool << "mul" << " " << "$" << register1 << " " << "$" << register2 << " " << "$" << register3;
            return pool.str();
        }
        return "";
    }

    string sub() {
        if (information == "") {
            stringstream pool;
            pool << "subu" << " " << "$" << register1 << " " << "$" << register2 << " " << "$" << register3;
            return pool.str();
        }
        return "";
    }

    string subi() {
        if (information == "") {
            stringstream pool;
            pool << "subiu" << " " << "$" << register1 << " " << "$" << register2 << " " << address;
            return pool.str();
        } else {
            return "";
        }
    }

    string div() {
        if (information == "") {
            stringstream pool;
            pool << "div" << " " << "$" << register1 << " " << "$" << register2 << " " << "$" << register3;
            return pool.str();
        } else if (information == "only") {
            stringstream pool;
            pool << "div" << " " << "$" << register1 << " " << "$" << register2;
            return pool.str();
        }
        return "";
    }

    string mfhi() {
        if (information == "") {
            stringstream pool;
            pool << "mfhi" << " " << "$" << register1;
            return pool.str();
        }
        return "";
    }

    string seq() {
        if (information == "") {
            stringstream pool;
            pool << "seq" << " " << "$" << register1 << " " << "$" << register2 << " " << "$" << register3;
            return pool.str();
        }
        return "";
    }

    string sne() {
        if (information == "") {
            stringstream pool;
            pool << "sne" << " " << "$" << register1 << " " << "$" << register2 << " " << "$" << register3;
            return pool.str();
        }
        return "";
    }

    string sgt() {
        if (information == "") {
            stringstream pool;
            pool << "sgt" << " " << "$" << register1 << " " << "$" << register2 << " " << "$" << register3;
            return pool.str();
        }
        return "";
    }

    string sge() {
        if (information == "") {
            stringstream pool;
            pool << "sge" << " " << "$" << register1 << " " << "$" << register2 << " " << "$" << register3;
            return pool.str();
        }
        return "";
    }

    string slt() {
        if (information == "") {
            stringstream pool;
            pool << "slt" << " " << "$" << register1 << " " << "$" << register2 << " " << "$" << register3;
            return pool.str();
        }
        return "";
    }

    string sle() {
        if (information == "") {
            stringstream pool;
            pool << "sle" << " " << "$" << register1 << " " << "$" << register2 << " " << "$" << register3;
            return pool.str();
        }
        return "";
    }

    string move() {
        if (information == "") {
            stringstream pool;
            pool << "move" << " " << "$" << register1 << " " << "$" << register2;
            return pool.str();
        }
        return "";
    }

    string li() {
        if (information == "") {
            stringstream pool;
            pool << "li" << " " << "$" << register1 << " " << address;
            return pool.str();
        }
        return "";
    }

    string syscall() {
        return "syscall";
    }

    string la() {
        stringstream pool;
        pool << "la" << " " << "$" << register1 << " " << information;
        return pool.str();
    }

    string label() {
        stringstream pool;
        pool << information << " : ";
        return pool.str();
    }

    string j() {
        stringstream pool;
        pool << "j" << " " << information;
        return pool.str();
    }

    string bne() {
        stringstream pool;
        pool << "bne" << " " << "$" << register1 << " " << "$" << register2 << " " << information;
        return pool.str();
    }

    string beq() {
        stringstream pool;
        pool << "beq" << " " << "$" << register1 << " " << "$" << register2 << " " << information;
        return pool.str();
    }

    string jal() {
        stringstream pool;
        pool << "jal" << " " << information;
        return pool.str();
    }

    string jr() {
        stringstream pool;
        pool << "jr" << " " << "$" << register1;
        return pool.str();
    }


};


class Coder {
private:

    map<int, int> addressCaches;

    map<int, int> registerBusy;

    map<int, int> arrayInformations;  // 0 : 常量数组 1：非参数变量数组  2：参数数组

    /**
     * 获取一个全局变量或者main中的变量对应的内存地址
     */
    int getGlobalVarAddress(int index) {
        if (addressCaches.count(index) == 1) {
            return addressCaches[index];
        } else {
            int nowAddress = 0;
            for (auto pair :globalDatas) {
                addressCaches[get<0>(pair)] = nowAddress;
                if (get<0>(pair) == index) {
                    return nowAddress;
                }
                nowAddress += get<1>(pair);
            }
            return -1; // 正常不会到这里
        }
    }

    int getFunctionVarAddress(int functionSection, int index) {
        int address1 = functionSections[functionSection]->getTempOffset(index);
        if (address1 != -1) {
            return address1;
        } else {
            return functionSections[functionSection]->getParaOffset(index);
        }
    }

    int allocateRegister() {
        for (int i = 0; i <= 31; i++) {
            if (registerBusy[i] == 0) {
                registerBusy[i] = 1;
                return i;
            }
        }

        cout << "错误！ 寄存器不够用了！" << endl;
        return -1;  // 正常不会到这里。
    }

    void freeRegister(int i) {
        registerBusy[i] = 0;
    }


    int loadValueToRegister(int valueIndex) {
        if (constValues.count(valueIndex)) {
            int register1 = allocateRegister();
            codes.emplace_back(make_shared<OneCode>("li", register1, -1, -1, constValues[valueIndex], ""));
            return register1;
        } else {
            if (varsBelongsTo[valueIndex] == -1) {  // 如果是全局变量，那么一定存储在全局数据区
                int globalAddress = getGlobalVarAddress(valueIndex);
                int register1 = allocateRegister();
                // lw $1 100($2)
                codes.emplace_back(make_shared<OneCode>("lw", register1, 28, -1, globalAddress, "offset"));
                return register1;
            } else {
                int functionAddressOffset = getFunctionVarAddress(varsBelongsTo[valueIndex], valueIndex);
                int register1 = allocateRegister();
                codes.emplace_back(make_shared<OneCode>("lw", register1, 30, -1, functionAddressOffset, "offset"));
                return register1;
            }
        }
    }

    void setRegisterToValue(int register1, int valueIndex) {
        if (varsBelongsTo[valueIndex] == -1) {  // 要给一个全局变量赋值
            int globalAddress = getGlobalVarAddress(valueIndex);
            codes.emplace_back(make_shared<OneCode>("sw", register1, 28, -1, globalAddress, "offset"));
        } else {
            int functionAddressOffset = getFunctionVarAddress(varsBelongsTo[valueIndex], valueIndex);
            codes.emplace_back(make_shared<OneCode>("sw", register1, 30, -1, functionAddressOffset, "offset"));
        }
    }

    void saveToGlobalOffset(int register1, int globalOffset) {
        codes.emplace_back(make_shared<OneCode>("sw", register1, 28, -1, globalOffset, "offset"));
    }

    void saveToFunctionOffset(int register1, int functionOffset) {
        codes.emplace_back(make_shared<OneCode>("sw", register1, 30, -1, functionOffset, "offset"));
    }

    void saveToRegisterOffset(int valueRegister, int baseRegister, int address) {
        codes.emplace_back(make_shared<OneCode>("sw", valueRegister, baseRegister, -1, address, "offset"));
    }

    void loadFromFunctionOffset(int register1, int offSet) {
        codes.emplace_back(make_shared<OneCode>("lw", register1, 30, -1, offSet, "offset"));
    }

    int loadFromFunctionOffset(int offset) {
        int register1 = allocateRegister();
        codes.emplace_back(make_shared<OneCode>("lw", register1, 30, -1, offset, "offset"));
        return register1;
    }

    int loadFromRegisterOffset(int baseRegister, int offSet) {
        int valueRegister = allocateRegister();
        codes.emplace_back(make_shared<OneCode>("lw", valueRegister, baseRegister, -1, offSet, "offset"));
        return valueRegister;
    }

    int shiftLeft(int register1, int len) {
        //sll $t1 $t2 10
        int register2 = allocateRegister();
        codes.emplace_back(make_shared<OneCode>("sll", register2, register1, -1, len, ""));
        return register2;
    }

    int add(int register1, int register2) {
        int register3 = allocateRegister();
        codes.emplace_back(make_shared<OneCode>("add", register3, register1, register2, -1, ""));
        return register3;
    }

    int addConst(int register1, int constValue) {
        int register3 = allocateRegister();
        codes.emplace_back(make_shared<OneCode>("addi", register3, register1, -1, constValue, ""));
        return register3;
    }

    void addConst(int resultRegister, int register1, int constValue) {
        codes.emplace_back(make_shared<OneCode>("addi", resultRegister, register1, -1, constValue, ""));
    }

    int mul(int register1, int register2) {
        int register3 = allocateRegister();
        codes.emplace_back(make_shared<OneCode>("mul", register3, register1, register2, -1, ""));
        return register3;
    }

    int sub(int register1, int register2) {
        int register3 = allocateRegister();
        codes.emplace_back(make_shared<OneCode>("sub", register3, register1, register2, -1, ""));
        return register3;
    }

    void subConst(int resultRegister, int register1, int constValue) {
        codes.emplace_back(make_shared<OneCode>("subi", resultRegister, register1, -1, constValue, ""));
    }

    int div(int register1, int register2) {
        int register3 = allocateRegister();
        codes.emplace_back(make_shared<OneCode>("div", register3, register1, register2, -1, ""));
        return register3;
    }

    int mode(int register1, int register2) {
        int register3 = allocateRegister();
        codes.emplace_back(make_shared<OneCode>("div", register1, register2, -1, -1, "only"));
        codes.emplace_back(make_shared<OneCode>("mfhi", register3, -1, -1, -1, ""));
        return register3;
    }

    int not_(int register1) {
        int register2 = allocateRegister();
        codes.emplace_back(make_shared<OneCode>("seq", register2, register1, 0, -1, ""));
        return register2;
    }

    int equal_(int register1, int register2) {
        int register3 = allocateRegister();
        codes.emplace_back(make_shared<OneCode>("seq", register3, register1, register2, -1, ""));
        return register3;
    }

    int not_equal(int register1, int register2) {
        int register3 = allocateRegister();
        codes.emplace_back(make_shared<OneCode>("sne", register3, register1, register2, -1, ""));
        return register3;
    }

    int gt(int register1, int register2) {
        int register3 = allocateRegister();
        codes.emplace_back(make_shared<OneCode>("sgt", register3, register1, register2, -1, ""));
        return register3;
    }

    int ge(int register1, int register2) {
        int register3 = allocateRegister();
        codes.emplace_back(make_shared<OneCode>("sge", register3, register1, register2, -1, ""));
        return register3;
    }

    int lt(int register1, int register2) {
        int register3 = allocateRegister();
        codes.emplace_back(make_shared<OneCode>("slt", register3, register1, register2, -1, ""));
        return register3;
    }

    int le(int register1, int register2) {
        int register3 = allocateRegister();
        codes.emplace_back(make_shared<OneCode>("sle", register3, register1, register2, -1, ""));
        return register3;
    }

    void moveTo(int toRegister, int fromRegister) {
        codes.emplace_back(make_shared<OneCode>("move", toRegister, fromRegister, -1, -1, ""));
    }

    void setConst(int register1, int constNumber) {
        codes.emplace_back(make_shared<OneCode>("li", register1, -1, -1, constNumber, ""));
    }

    int setConst(int constNumber) {
        int register1 = allocateRegister();
        setConst(register1, constNumber);
        return register1;
    }

    void syscall(int number) {
        setConst(2, number);
        codes.emplace_back(make_shared<OneCode>("syscall", -1, -1, -1, -1, ""));
    }


    void setLabel(int labelIndex) {
        stringstream pool;
        pool << "label_" << labelIndex;
        codes.emplace_back(make_shared<OneCode>("label", -1, -1, -1, -1, pool.str()));
    }

    void jumpTo(int labelIndex) {
        stringstream pool;
        pool << "label_" << labelIndex;
        codes.emplace_back(make_shared<OneCode>("j", -1, -1, -1, -1, pool.str()));
    }

    void jumpIf(int register1, int labelIndex) {
        stringstream pool;
        pool << "label_" << labelIndex;
        // 非零的值都认为是真值
        codes.emplace_back(make_shared<OneCode>("bne", register1, 0, -1, -1, pool.str()));
    }

    void jumpIfNot(int register1, int labelIndex) {
        stringstream pool;
        pool << "label_" << labelIndex;
        //  非0的值都是真值，都不跳转
        codes.emplace_back(make_shared<OneCode>("beq", register1, 0, -1, -1, pool.str()));
    }

    void jumpAndLink(int labelIndex) {
        stringstream pool;
        pool << "label_" << labelIndex;
        codes.emplace_back(make_shared<OneCode>("jal", -1, -1, -1, -1, pool.str()));
    }

    void jumpToRegister(int register1) {
        codes.emplace_back(make_shared<OneCode>("jr", register1, -1, -1, -1, ""));
    }

public:

    /**
     *            指令     寄存器1    寄存器2    寄存器3    其他信息
     */
    vector<shared_ptr<OneCode> > codes;  // 即将输出的mips代码，可供最后优化

    /**
     * 常量的值
     */
    map<int, int> constValues;

    /**
     * 各函数对应的动态内存AP
     * 包括函数内部的变量、变量数组
     */
    map<int, shared_ptr<Memory> > functionSections;

    /**
     * 全局数据，包括所有常量数组、字符串;全局变量、变量数组;main函数里的变量、变量数组。
     * <index,length>
     */
    vector<tuple<int, int> > globalDatas;

    map<int, vector<int> > constArrays;  // 所有全局常量数组中存储的值。最后要输出到.data


    /**
     * 各个变量分别属于哪个函数。-1表示全局或main。
     * 这里不包含常数数组的信息。因为常熟数组一定在.data部分。
     * 不包含普通常数，因为普通常数要解读为字面量。
     */
    map<int, int> varsBelongsTo;

    int nowFunctionIndex = -1;  // 当前在哪个函数里，如果不在任何函数里，那么就为-1，代表全局。main不算函数。

    Coder() {
        for (int i = 0; i <= 31; i++) {
            registerBusy[i] = 0;  // 默认寄存器空闲。
        }
        /**
         *  0:常量0
         *  1:汇编器
         *
         */
        for (int i :{0, 1, 2, 4, 28, 29, 30, 31}) {
            registerBusy[i] = 1;
        }

        setConst(28, 268500992);  // 把全局寄存器设置到固定位置
        setConst(30, 268697600);
    }

    void solveDeclaration(int i) {
        shared_ptr<Stm> &ptr = Stm::allStms[i];
        int index = ptr->result;
        if (ptr->operation == "temp") {     // 计算过程中的临时变量，需要有对应的内存地址
            if (nowFunctionIndex == -1) {
                globalDatas.emplace_back(tuple<int, int>(index, 4));
                varsBelongsTo[index] = -1;
            } else {
                functionSections[nowFunctionIndex]->temps.emplace_back(tuple<int, int>(index, 4));
                varsBelongsTo[index] = nowFunctionIndex;
            }
        } else if (ptr->operation == "ConstUnarray") {
            constValues[ptr->result] = ptr->numberA;
        } else if (ptr->operation == "ConstArray") {
            //  常数数组统一存到全局空间
            globalDatas.emplace_back(tuple<int, int>(ptr->result, ptr->numberA * 4));
            arrayInformations[ptr->result] = 0;
            varsBelongsTo[ptr->result] = -1;  // 常量数组也“属于”全局数据区
        } else if (ptr->operation == "VarUnarray") {
            if (nowFunctionIndex == -1) {
                globalDatas.emplace_back(tuple<int, int>(index, 4));
                varsBelongsTo[index] = -1;
            } else {
                functionSections[nowFunctionIndex]->temps.emplace_back(tuple<int, int>(index, 4));
                varsBelongsTo[index] = nowFunctionIndex;
            }
        } else if (ptr->operation == "VarArray") {
            if (nowFunctionIndex == -1) {
                globalDatas.emplace_back(tuple<int, int>(index, ptr->numberA * 4));
                varsBelongsTo[index] = -1;
            } else {
                functionSections[nowFunctionIndex]->temps.emplace_back(tuple<int, int>(index, ptr->numberA * 4));
                varsBelongsTo[index] = nowFunctionIndex;
            }
            arrayInformations[index] = 1;  // 非参数数组
        } else if (ptr->operation == "ParaUnarray") {   // 参数的大小均为4
            functionSections[nowFunctionIndex]->temps.emplace_back(tuple<int, int>(index, 4));
            varsBelongsTo[index] = nowFunctionIndex;
        } else if (ptr->operation == "ParaArray") {     // 参数的大小均为4
            functionSections[nowFunctionIndex]->temps.emplace_back(tuple<int, int>(index, 4));
            varsBelongsTo[index] = nowFunctionIndex;
            arrayInformations[index] = 2;  // 参数数组
        }
    }

    int solveAboutFunction(int i) {
        shared_ptr<Stm> &ptr = Stm::allStms[i];
        if (ptr->type == "setMain") {

        } else if (ptr->type == "return") {
            if (ptr->operation == "main") {
                syscall(10);
            } else if (ptr->operation == "") {
                int returnValueIndex = ptr->result;
                if (returnValueIndex != -1) {  // 有返回值
                    int returnRegister = loadValueToRegister(returnValueIndex);
                    saveToFunctionOffset(returnRegister, 0);  // 存储返回值到内存
                    freeRegister(returnRegister);
                }
                loadFromFunctionOffset(31, 4);
                loadFromFunctionOffset(30, 8);
                jumpToRegister(31);  // 这里可能可以优化
            }
        } else if (ptr->type == "setPara") {
            int offSet;
            if (nowFunctionIndex == -1) {  // 如果在main函数里调用某个函数，那么当前需要从偏移0开始构造活动记录
                offSet = 0;
            } else {
                offSet = functionSections[nowFunctionIndex]->getAllOffSet();
            }

            offSet += 8;  // 越过返回值、返回地址。因为返回地址需要函数自己维护。
            offSet += 4;  // 再越过abp之后，才到参数区
            int register2 = loadValueToRegister(ptr->result);
            saveToFunctionOffset(register2, offSet);
            offSet += 4;
            freeRegister(register2);
            int j = i + 1;
            for (; Stm::allStms[j]->type == "setPara"; j++) {
                shared_ptr<Stm> ptr2 = Stm::allStms[j];
                int register3 = loadValueToRegister(ptr2->result);
                saveToFunctionOffset(register3, offSet);
                offSet += 4;
                freeRegister(register3);
            }
            return j;
        } else if (ptr->type == "callFunction") {
            int offSet;
            if (nowFunctionIndex == -1) {  // 如果在main函数里调用某个函数，那么当前需要从偏移0开始构造活动记录
                offSet = 0;
            } else {
                offSet = functionSections[nowFunctionIndex]->getAllOffSet();
            }
            offSet += 8;  // 越过返回值、返回地址。因为返回地址需要函数自己维护。
            saveToFunctionOffset(30, offSet);  // apb
            if (nowFunctionIndex == -1) {

            } else {
                addConst(30, 30, functionSections[nowFunctionIndex]->getAllOffSet());
            }
            jumpAndLink(ptr->numberA);
            if (nowFunctionIndex == -1) {  // 如果在main函数里调用某个函数，那么当前需要从偏移0开始构造活动记录
                offSet = 0;
            } else {
                offSet = functionSections[nowFunctionIndex]->getAllOffSet();
            }
            if (ptr->result != -1) {
                int valueRegister = loadFromFunctionOffset(offSet);
                setRegisterToValue(valueRegister, ptr->result);
                freeRegister(valueRegister);
            }
        } else if (ptr->type == "function") {
            if (ptr->operation == "start") {
                nowFunctionIndex = ptr->result;
                setLabel(nowFunctionIndex);
                saveToFunctionOffset(31, 4);  // 保存返回地址
                shared_ptr<Memory> newFunction = make_shared<Memory>();
                functionSections[nowFunctionIndex] = newFunction;
            } else if (ptr->operation == "end") {
                nowFunctionIndex = -1;
            }
        }
        return -1;
    }

    void solveAssign(int i) {
        shared_ptr<Stm> &ptr = Stm::allStms[i];
        if (ptr->operation == "ConstArray") {
            if (constArrays.count(ptr->result) == 0) {
                constArrays[ptr->result] = {};
            }
            constArrays[ptr->result].emplace_back(ptr->numberA);
        } else if (ptr->operation == "VarUnarrayInitial") {
            int varIndex = ptr->result;
            int valueIndex = ptr->numberA;
            // 用一个变量的值给令一个变量赋值
            int register1 = loadValueToRegister(valueIndex);  // register1存储了要赋的值
            setRegisterToValue(register1, varIndex);
            freeRegister(register1);
        } else if (ptr->operation == "VarArrayInitial") {
            int varArrayIndex = ptr->result;
            int valueIndex = ptr->numberA;
            int number = ptr->numberB;
            int addressOffset;
            if (varsBelongsTo[varArrayIndex] == -1) {
                addressOffset = getGlobalVarAddress(varArrayIndex);
            } else {
                addressOffset = getFunctionVarAddress(varsBelongsTo[varArrayIndex], varArrayIndex);
            }
            addressOffset += 4 * number;
            int register1 = loadValueToRegister(valueIndex);
            if (varsBelongsTo[varArrayIndex] == -1) {
                saveToGlobalOffset(register1, addressOffset);
            } else {
                saveToFunctionOffset(register1, addressOffset);
            }
            freeRegister(register1);
        } else if (ptr->operation == "Unarray") {
            int varIndex = ptr->result;
            int valueIndex = ptr->numberA;
            int register1 = loadValueToRegister(valueIndex);
            setRegisterToValue(register1, varIndex);
            freeRegister(register1);
        } else if (ptr->operation == "Array") {
            int arrayIndex = ptr->result;   // 要给哪个数组赋值
            int offSetIndex = ptr->numberA;
            int valueIndex = ptr->numberB;
            if (arrayInformations[arrayIndex] == 2) {  // 如果这个所谓的数组是参数数组
                int arrayAddressRegister = loadValueToRegister(arrayIndex);  // 数组的真实地址
                int offSetRegister = loadValueToRegister(offSetIndex);
                int realOffSetRegister = shiftLeft(offSetRegister, 2);
                freeRegister(offSetRegister);
                int realAddressRegister = add(arrayAddressRegister, realOffSetRegister);
                freeRegister(arrayAddressRegister);
                freeRegister(realOffSetRegister);
                int valueRegister = loadValueToRegister(valueIndex);
                saveToRegisterOffset(valueRegister, realAddressRegister, 0);
                freeRegister(valueRegister);
                freeRegister(realAddressRegister);
            } else {
                int register1 = loadValueToRegister(offSetIndex);
                int register2 = shiftLeft(register1, 2);
                freeRegister(register1);
                int baseRegister;
                if (varsBelongsTo[arrayIndex] == -1) {
                    baseRegister = add(28, register2);
                } else {
                    baseRegister = add(30, register2);
                }
                freeRegister(register2);
                int arrayOffset;
                if (varsBelongsTo[arrayIndex] == -1) {
                    arrayOffset = getGlobalVarAddress(arrayIndex);
                } else {
                    arrayOffset = getFunctionVarAddress(varsBelongsTo[arrayIndex], arrayIndex);
                }
                int valueRegister = loadValueToRegister(valueIndex);
                saveToRegisterOffset(valueRegister, baseRegister, arrayOffset);
                freeRegister(valueRegister);
                freeRegister(baseRegister);
            }
        }
    }

    void solveIO(int i) {
        shared_ptr<Stm> &ptr = Stm::allStms[i];
        if (ptr->type == "print") {
            if (ptr->operation == "value") {
                int valueIndex = ptr->result;
                int register1 = loadValueToRegister(valueIndex);
                moveTo(4, register1);
                freeRegister(register1);
                syscall(1);
            } else if (ptr->operation == "str") {
                int strIndex = ptr->result;
                string str_to_print = Stm::int2str[strIndex];  // 要打印的字符串
                int len = (int) str_to_print.size() + 1;
                for (char ch:str_to_print) {
                    if (ch == '\\') {  // 转义字符会重复计算，需要弄掉。
                        len--;
                    }
                }

                if ((len) % 4 != 0) {
                    int remain = 4 - (len) % 4;  // 剩余几个位置
                    len = len + remain;
                    if (remain == 1) {
                        len += 4;
                    }
                }
                globalDatas.emplace_back(tuple<int, int>(strIndex, len));
                addConst(4, 28, getGlobalVarAddress(strIndex));
                syscall(4);
            }
        } else if (ptr->type == "getInt") {
            syscall(5);
            int targetIndex = ptr->result;
            setRegisterToValue(2, targetIndex);
        }
    }

    void solveOperate(int i) {  // + - * / %
        shared_ptr<Stm> &ptr = Stm::allStms[i];
        if (ptr->type == "mul") {
            if (ptr->operation == "Const") {
                int register1 = setConst(ptr->numberB);
                int register2 = loadValueToRegister(ptr->numberA);
                int register3 = mul(register1, register2);
                freeRegister(register1);
                freeRegister(register2);
                setRegisterToValue(register3, ptr->result);
                freeRegister(register3);
            } else if (ptr->operation == "") {
                int register1 = loadValueToRegister(ptr->numberA);
                int register2 = loadValueToRegister(ptr->numberB);
                int register3 = mul(register1, register2);
                freeRegister(register1);
                freeRegister(register2);
                setRegisterToValue(register3, ptr->result);
                freeRegister(register3);
            }
        } else if (ptr->type == "add") {
            int register1 = loadValueToRegister(ptr->numberA);
            int register2 = loadValueToRegister(ptr->numberB);
            int register3 = add(register1, register2);
            freeRegister(register1);
            freeRegister(register2);
            setRegisterToValue(register3, ptr->result);
            freeRegister(register3);
        } else if (ptr->type == "sub") {
            if (ptr->operation == "ConstSub") {
                int register1 = setConst(ptr->numberA);
                int register2 = loadValueToRegister(ptr->numberB);
                int register3 = sub(register1, register2);
                freeRegister(register1);
                freeRegister(register2);
                setRegisterToValue(register3, ptr->result);
                freeRegister(register3);
            } else if (ptr->operation == "") {
                int register1 = loadValueToRegister(ptr->numberA);
                int register2 = loadValueToRegister(ptr->numberB);
                int register3 = sub(register1, register2);
                freeRegister(register1);
                freeRegister(register2);
                setRegisterToValue(register3, ptr->result);
                freeRegister(register3);
            }
        } else if (ptr->type == "div") {
            int register1 = loadValueToRegister(ptr->numberA);
            int register2 = loadValueToRegister(ptr->numberB);
            int register3 = div(register1, register2);
            freeRegister(register1);
            freeRegister(register2);
            setRegisterToValue(register3, ptr->result);
            freeRegister(register3);
        } else if (ptr->type == "mode") {
            int register1 = loadValueToRegister(ptr->numberA);
            int register2 = loadValueToRegister(ptr->numberB);
            int register3 = mode(register1, register2);
            freeRegister(register1);
            freeRegister(register2);
            setRegisterToValue(register3, ptr->result);
            freeRegister(register3);
        }
    }

    void solveLogisticOperate(int i) {
        shared_ptr<Stm> &ptr = Stm::allStms[i];
        if (ptr->type == "not") {
            int register1 = loadValueToRegister(ptr->numberA);
            int register2 = not_(register1);
            freeRegister(register1);
            setRegisterToValue(register2, ptr->result);
            freeRegister(register2);
        } else if (ptr->type == "equalCheck") {
            int register1 = loadValueToRegister(ptr->numberA);
            int register2 = loadValueToRegister(ptr->numberB);
            int register3 = equal_(register1, register2);
            freeRegister(register1);
            freeRegister(register2);
            setRegisterToValue(register3, ptr->result);
            freeRegister(register3);
        } else if (ptr->type == "notEqualCheck") {
            int register1 = loadValueToRegister(ptr->numberA);
            int register2 = loadValueToRegister(ptr->numberB);
            int register3 = not_equal(register1, register2);
            freeRegister(register1);
            freeRegister(register2);
            setRegisterToValue(register3, ptr->result);
            freeRegister(register3);
        } else if (ptr->type == "gt") {
            int register1 = loadValueToRegister(ptr->numberA);
            int register2 = loadValueToRegister(ptr->numberB);
            int register3 = gt(register1, register2);
            freeRegister(register1);
            freeRegister(register2);
            setRegisterToValue(register3, ptr->result);
            freeRegister(register3);
        } else if (ptr->type == "ge") {
            int register1 = loadValueToRegister(ptr->numberA);
            int register2 = loadValueToRegister(ptr->numberB);
            int register3 = ge(register1, register2);
            freeRegister(register1);
            freeRegister(register2);
            setRegisterToValue(register3, ptr->result);
            freeRegister(register3);
        } else if (ptr->type == "lt") {
            int register1 = loadValueToRegister(ptr->numberA);
            int register2 = loadValueToRegister(ptr->numberB);
            int register3 = lt(register1, register2);
            freeRegister(register1);
            freeRegister(register2);
            setRegisterToValue(register3, ptr->result);
            freeRegister(register3);
        } else if (ptr->type == "le") {
            int register1 = loadValueToRegister(ptr->numberA);
            int register2 = loadValueToRegister(ptr->numberB);
            int register3 = le(register1, register2);
            freeRegister(register1);
            freeRegister(register2);
            setRegisterToValue(register3, ptr->result);
            freeRegister(register3);
        }
    }

    void solveBranch(int i) {
        shared_ptr<Stm> &ptr = Stm::allStms[i];
        if (ptr->type == "label") {
            setLabel(ptr->result);
        } else if (ptr->type == "jump") {
            jumpTo(ptr->result);
        } else if (ptr->type == "jumpIf") {
            int register1 = loadValueToRegister(ptr->numberA);
            jumpIf(register1, ptr->result);
            freeRegister(register1);
        } else if (ptr->type == "jumpIfNot") {
            int register1 = loadValueToRegister(ptr->numberA);
            jumpIfNot(register1, ptr->result);
            freeRegister(register1);
        }
    }


    void solveAboutArray(int i) {
        shared_ptr<Stm> &ptr = Stm::allStms[i];
        if (ptr->type == "loadFrom") {
            int resultIndex = ptr->result;
            int arrayIndex = ptr->numberA;
            int offsetIndex = ptr->numberB;
            if (arrayInformations[arrayIndex] == 2) {  // 如果这个数组是参数数组
                int offsetRegister = loadValueToRegister(offsetIndex);
                int realOffsetRegister = shiftLeft(offsetRegister, 2);
                freeRegister(offsetRegister);
                int arrayRegister = loadValueToRegister(arrayIndex);
                int realAddressRegister = add(arrayRegister, realOffsetRegister);
                freeRegister(arrayRegister);
                freeRegister(realOffsetRegister);
                int valueRegister = loadFromRegisterOffset(realAddressRegister, 0);
                freeRegister(realAddressRegister);
                setRegisterToValue(valueRegister, resultIndex);
                freeRegister(valueRegister);
            } else {  // 数组非参数
                if (varsBelongsTo[arrayIndex] == -1) {  // 这是一个全局数组
                    int arrayAddress = getGlobalVarAddress(arrayIndex);  // 相对于全局基地址的偏移
                    int offsetRegister = loadValueToRegister(offsetIndex);
                    int offset_2 = shiftLeft(offsetRegister, 2);  // 左移两位后的结果
                    freeRegister(offsetRegister);
                    int realAddress2 = add(28, offset_2);
                    freeRegister(offset_2);
                    int resultRegister = loadFromRegisterOffset(realAddress2, arrayAddress);
                    freeRegister(realAddress2);
                    setRegisterToValue(resultRegister, resultIndex);
                    freeRegister(resultRegister);
                } else {    // 这是一个当前动态区里的数组
                    int arrayAddress = getFunctionVarAddress(varsBelongsTo[arrayIndex], arrayIndex);
                    int offsetRegister = loadValueToRegister(offsetIndex);
                    int offset_2 = shiftLeft(offsetRegister, 2);  // 左移
                    freeRegister(offsetRegister);
                    int realAddress2 = add(30, offset_2);
                    freeRegister(offset_2);
                    int resultRegister = loadFromRegisterOffset(realAddress2, arrayAddress);
                    freeRegister(realAddress2);
                    setRegisterToValue(resultRegister, resultIndex);
                    freeRegister(resultRegister);
                }
            }
        } else if (ptr->type == "calculateAddress") {
            int resultIndex = ptr->result;
            int arrayIndex = ptr->numberA;
            int offsetIndex = ptr->numberB;
            if (arrayInformations[arrayIndex] == 2) {   // 这是一个参数数组
                int register2 = loadValueToRegister(offsetIndex);
                int register3 = shiftLeft(register2, 2);
                freeRegister(register2);
                int register1 = loadValueToRegister(arrayIndex);
                int newAddressRegister = add(register1, register3);
                freeRegister(register1);
                freeRegister(register3);
                setRegisterToValue(newAddressRegister, resultIndex);
                freeRegister(newAddressRegister);
            } else {
                if (varsBelongsTo[arrayIndex] == -1) {   // 这是一个全局数组
                    int arrayOffsetToGlobal = getGlobalVarAddress(arrayIndex);
                    int offsetRegister = loadValueToRegister(offsetIndex);
                    int realOffset = shiftLeft(offsetRegister, 2);
                    freeRegister(offsetRegister);
                    int add_result_register = add(realOffset, 28);
                    freeRegister(realOffset);
                    int newAddressRegister = addConst(add_result_register, arrayOffsetToGlobal);
                    freeRegister(add_result_register);
                    setRegisterToValue(newAddressRegister, resultIndex);
                    freeRegister(newAddressRegister);
                } else {  // 这是某个函数里的数组
                    int arrayOffsetToFunction = getFunctionVarAddress(varsBelongsTo[arrayIndex],
                                                                      arrayIndex);  // 相对于函数运行栈基地址的偏移
                    int offsetRegister = loadValueToRegister(offsetIndex);
                    int realOffset = shiftLeft(offsetRegister, 2);
                    freeRegister(offsetRegister);
                    int add_result_register = add(realOffset, 30);
                    freeRegister(realOffset);
                    int newAddressRegister = addConst(add_result_register, arrayOffsetToFunction);
                    freeRegister(add_result_register);
                    setRegisterToValue(newAddressRegister, resultIndex);
                    freeRegister(newAddressRegister);
                }
            }
        }
    }

    void make() {
        for (int i = 0; i < Stm::allStms.size(); i++) {
            shared_ptr<Stm> &ptr = Stm::allStms[i];
            if (ptr->type == "declaration") {
                solveDeclaration(i);
            } else if (ptr->type == "assign") {
                solveAssign(i);
            } else if (ptr->type == "setMain" || ptr->type == "return" || ptr->type == "setPara" ||
                       ptr->type == "callFunction" || ptr->type == "function") {
                int j = solveAboutFunction(i);
                if (j != -1) {
                    i = j - 1;
                    continue;
                }
            } else if (ptr->type == "label" || ptr->type == "jump" || ptr->type == "jumpIf" ||
                       ptr->type == "jumpIfNot") {
                solveBranch(i);
            } else if (ptr->type == "print" || ptr->type == "getInt") {
                solveIO(i);
            } else if (ptr->type == "mul" || ptr->type == "add" || ptr->type == "sub" || ptr->type == "div" ||
                       ptr->type == "mode") {
                solveOperate(i);
            } else if (ptr->type == "not" || ptr->type == "lt" || ptr->type == "le" || ptr->type == "equalCheck" ||
                       ptr->type == "notEqualCheck" || ptr->type == "gt" || ptr->type == "ge") {
                solveLogisticOperate(i);
            } else if (ptr->type == "loadFrom" || ptr->type == "calculateAddress") {
                solveAboutArray(i);
            }
        }
    }

    void outPut(string filePath) {
        ofstream outFile(filePath, ios::out);
        outFile << ".data" << endl;
        for (auto par :globalDatas) {
            int index = get<0>(par);
            if (constArrays.count(index)) {  // 这是一个常量数组
                outFile << ".word";
                vector<int> &numbers = constArrays[index];
                for (auto number : numbers) {
                    outFile << " " << number;
                }
                outFile << endl;
            } else if (arrayInformations.count(index)) {  // 全局变量数组或者mian变量数组
                outFile << ".space " << get<1>(par);
                outFile << endl;
            } else if (Stm::int2str.count(index)) {  // 这是一个字符串。
                string str = Stm::int2str[index];
                outFile << ".asciiz \"" << str << "\"" << endl;
                int len = (int) str.size() + 1;
                for (char ch :str) {
                    if (ch == '\\') {  // 避免转义字符重复计算。
                        len--;
                    }
                }
                int remain = 4 - len % 4;
                if (remain == 2) {
                    outFile << ".asciiz " << "\" \"" << endl;
                } else if (remain == 3) {
                    outFile << ".asciiz " << "\"  \"" << endl;
                } else if (remain == 1) {
                    outFile << ".asciiz " << "\"    \"" << endl;
                }
            } else {  // 一定是全局变量
                outFile << ".space " << 4;
                outFile << endl;
            }
        }

        outFile << ".text" << endl;
        for (auto &code:codes) {
            outFile << code->toStr() << endl;
        }
        outFile.close();
    }

};


#endif //COMPILER_CODER_H
