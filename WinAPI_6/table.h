#pragma once


enum class UIitems {
	comboTables,
	comboFields,
	buttonOk,
	buttonNext,
	buttonExecute,
	comboConditionsOperators,
	comboLogicOperators,
	editField,
	resultTable,
	multilineField,
	menu,
	errorMsg,
	buttonSelect,
	buttonDelete,
	buttonUpdate,
	buttonInsert,
	archUp,
	archDown,
	archExec,
	archExit,
	archOk,
	archList,
	archWindow,
};

enum class States {
	s,
	s1,
	s2,
	s3,
	s4,
	s5,
	s6,
	s7,
	s8,
	s9,
	s10,
	s11,
	s12,
	s13,
	s14,
	s15,
	s16_1,
	s16_2,
	f,
};

enum class Actions {
	emptyA,
	//execSelect,
	resetDataAndMoveToRequestConstruct,
	resetDataAndMoveToSelectTable,
	showResult,
	showError,
	closeErrorWindow,
	clearTable,
	startBuildSelect,
	startBuildDelete,
	startBuildUpdate,
	startBuildInsert,
	addFieldToRequest,
	startBuildWhereSection,
	insertFieldValue,
	execRequest,
	insertComparisonOperator,
	insertFieldValueInWhere,
	insertLogicOperator,
	saveSelTable,
	saveSelField,
	saveSelLogicOp,
	saveSelCompareOp,
	resetDataAndMoveToArchive,
	moveUpSelection,
	moveDownSelection,
	closeArch,
	//insertFieldInRequest,
	//insertFieldValue,
	//startBuildWhereSection,
	//execRequest,
	//resetDataAndMoveToRequestConstruct,
	//resetDataAndMoveToSelectTable,
};

enum class Signals {
	emptyS,
	ok,
	next,
	select_,
	delete_,
	update,
	insert,
	execute,
	menuRequestItem,
	menuTableItem,
	success,
	error,
	selTable,
	selField,
	selLogicOp,
	selCompareOp,
	menuArchItem,
	archDown,
	archUp,
	archExecute,
	archOk,
	archExit,
};

//1.	Выпадающий список таблиц
//2.	Выпадающий список полей
//3.	Кнопка ОК
//4.	Кнопка ДАЛЕЕ
//5.	Кнопка ВЫПОЛНИТЬ
//6.	Выпадающий список условий
//7.	Выпадающий список логических операторов
//8.	Поле ввода значения
//9.	Таблица с результатом запроса
//10.	Многострочное текстовое поле с текстом запроса
//11.	Меню
//12.	Сообщение об ошибке
//13.	Кнопка SELECT
//14.	Кнопка DELETE
//15.	Кнопка UPDATE
//16.	Кнопка INSERT



char contextTable[19][23] = {
	//			  1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16  17	  18	19	20	 21	  22   23
	/*s*/		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '+', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
	/*1*/		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '+', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
	/*2*/		{'+', ' ', ' ', ' ', '+', ' ', ' ', ' ', ' ', ' ', '+', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
	/*3*/		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '+', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
	/*4*/		{' ', ' ', '+', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '+', '+', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
	/*5*/		{' ', ' ', '+', ' ', ' ', ' ', ' ', ' ', '-', ' ', '+', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
	/*6*/		{'+', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '-', '+', ' ', '+', '+', '+', '+', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
	/*7*/		{' ', '+', '+', '+', ' ', ' ', ' ', ' ', ' ', '-', '+', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
	/*8*/		{' ', ' ', '+', ' ', ' ', ' ', ' ', '+', ' ', '-', '+', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
	/*9*/		{' ', '+', '+', ' ', '+', ' ', ' ', ' ', ' ', '-', '+', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
	/*10*/		{' ', ' ', '+', ' ', ' ', '+', ' ', ' ', ' ', '-', '+', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
	/*11*/		{' ', ' ', '+', ' ', ' ', ' ', ' ', '+', ' ', '-', '+', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
	/*12*/		{' ', ' ', '+', ' ', ' ', ' ', '+', ' ', ' ', '-', '+', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
	/*13*/		{' ', '+', '+', ' ', '+', ' ', ' ', ' ', ' ', '-', '+', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
	/*14*/		{' ', ' ', '+', ' ', ' ', ' ', ' ', '+', ' ', '-', '+', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
	/*15*/		{' ', '+', '+', '+', ' ', ' ', ' ', ' ', ' ', '-', '+', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
	/*16_1*/	{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '+', ' ', ' ', ' ', ' ', ' ', '+', '+', '+', '+', ' ', '-', '+'},
	/*16_2*/	{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '-', ' ', '+', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '+', ' ', '+'},
	/*f*/		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
};


//0 - emptyA
//1 ok button
//2 next button
//3 select button
//4 delete button
//5 update button
//6 insert button
//7 execute button
//8 request menu item
//9 table menu item
//10 success
//11 error
//



Actions actionTable[19][23] = {
	//			0				 1											 2								3									4								5								6								7									8															9						10								11					12						13						14							15					16												17							18						19					20					21					22			
/*s*/	{Actions::emptyA, Actions::emptyA,						Actions::emptyA,						Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,			Actions::emptyA,								Actions::emptyA,							Actions::emptyA,			Actions::emptyA,		Actions::emptyA,		Actions::emptyA,		Actions::emptyA,			Actions::emptyA,			Actions::emptyA,					Actions::emptyA,			Actions::emptyA,			Actions::emptyA,		Actions::emptyA, Actions::emptyA, Actions::emptyA,},
/*1*/	{Actions::emptyA, Actions::emptyA,						Actions::emptyA,						Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,			Actions::resetDataAndMoveToRequestConstruct,	Actions::resetDataAndMoveToSelectTable,		Actions::emptyA,			Actions::emptyA,		Actions::emptyA,		Actions::emptyA,		Actions::emptyA,			Actions::emptyA,			Actions::resetDataAndMoveToArchive, Actions::emptyA,			Actions::emptyA,			Actions::emptyA,		Actions::emptyA, Actions::emptyA, Actions::emptyA,},
/*2*/	{Actions::emptyA, Actions::emptyA,						Actions::emptyA,						Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::execRequest,		Actions::resetDataAndMoveToRequestConstruct,	Actions::resetDataAndMoveToSelectTable,		Actions::emptyA,			Actions::emptyA,		Actions::saveSelTable,	Actions::emptyA,		Actions::emptyA,			Actions::emptyA,			Actions::resetDataAndMoveToArchive, Actions::emptyA,			Actions::emptyA,			Actions::emptyA,		Actions::emptyA, Actions::emptyA, Actions::emptyA,},
/*3*/	{Actions::emptyA, Actions::emptyA,						Actions::emptyA,						Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,			Actions::resetDataAndMoveToRequestConstruct,	Actions::resetDataAndMoveToSelectTable,		Actions::showResult,		Actions::showError,		Actions::emptyA,		Actions::emptyA,		Actions::emptyA,			Actions::emptyA,			Actions::resetDataAndMoveToArchive, Actions::emptyA,			Actions::emptyA,			Actions::emptyA,		Actions::emptyA, Actions::emptyA, Actions::emptyA,},
/*4*/	{Actions::emptyA, Actions::closeErrorWindow,			Actions::emptyA,						Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,			Actions::resetDataAndMoveToRequestConstruct,	Actions::resetDataAndMoveToSelectTable,		Actions::emptyA,			Actions::emptyA,		Actions::emptyA,		Actions::emptyA,		Actions::emptyA,			Actions::emptyA,			Actions::resetDataAndMoveToArchive, Actions::emptyA,			Actions::emptyA,			Actions::emptyA,		Actions::emptyA, Actions::emptyA, Actions::emptyA,},
/*5*/	{Actions::emptyA, Actions::clearTable,					Actions::emptyA,						Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,			Actions::resetDataAndMoveToRequestConstruct,	Actions::resetDataAndMoveToSelectTable,		Actions::emptyA,			Actions::emptyA,		Actions::emptyA,		Actions::emptyA,		Actions::emptyA,			Actions::emptyA,			Actions::resetDataAndMoveToArchive, Actions::emptyA,			Actions::emptyA,			Actions::emptyA,		Actions::emptyA, Actions::emptyA, Actions::emptyA,},
/*6*/	{Actions::emptyA, Actions::emptyA,						Actions::emptyA,						Actions::startBuildSelect,		Actions::startBuildDelete,		Actions::startBuildUpdate,		Actions::startBuildInsert,		Actions::emptyA,			Actions::resetDataAndMoveToRequestConstruct,	Actions::resetDataAndMoveToSelectTable,		Actions::emptyA,			Actions::emptyA,		Actions::saveSelTable,	Actions::emptyA,		Actions::emptyA,			Actions::emptyA,			Actions::resetDataAndMoveToArchive, Actions::emptyA,			Actions::emptyA,			Actions::emptyA,		Actions::emptyA, Actions::emptyA, Actions::emptyA,},
/*7*/	{Actions::emptyA, Actions::addFieldToRequest,			Actions::emptyA,						Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,			Actions::resetDataAndMoveToRequestConstruct,	Actions::resetDataAndMoveToSelectTable,		Actions::emptyA,			Actions::emptyA,		Actions::emptyA,		Actions::saveSelField,	Actions::emptyA,			Actions::emptyA,			Actions::resetDataAndMoveToArchive, Actions::emptyA,			Actions::emptyA,			Actions::emptyA,		Actions::emptyA, Actions::emptyA, Actions::emptyA,},
/*8*/	{Actions::emptyA, Actions::insertFieldValue,			Actions::emptyA,						Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,			Actions::resetDataAndMoveToRequestConstruct,	Actions::resetDataAndMoveToSelectTable,		Actions::emptyA,			Actions::emptyA,		Actions::emptyA,		Actions::emptyA,		Actions::emptyA,			Actions::emptyA,			Actions::resetDataAndMoveToArchive, Actions::emptyA,			Actions::emptyA,			Actions::emptyA,		Actions::emptyA, Actions::emptyA, Actions::emptyA,},
/*9*/	{Actions::emptyA, Actions::startBuildWhereSection,		Actions::emptyA,						Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::execRequest,		Actions::resetDataAndMoveToRequestConstruct,	Actions::resetDataAndMoveToSelectTable,		Actions::emptyA,			Actions::emptyA,		Actions::emptyA,		Actions::saveSelField,	Actions::emptyA,			Actions::emptyA,			Actions::resetDataAndMoveToArchive, Actions::emptyA,			Actions::emptyA,			Actions::emptyA,		Actions::emptyA, Actions::emptyA, Actions::emptyA,},
/*10*/	{Actions::emptyA, Actions::insertComparisonOperator,	Actions::emptyA,						Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,			Actions::resetDataAndMoveToRequestConstruct,	Actions::resetDataAndMoveToSelectTable,		Actions::emptyA,			Actions::emptyA,		Actions::emptyA,		Actions::emptyA,		Actions::emptyA,			Actions::saveSelCompareOp,	Actions::resetDataAndMoveToArchive, Actions::emptyA,			Actions::emptyA,			Actions::emptyA,		Actions::emptyA, Actions::emptyA, Actions::emptyA,},
/*11*/	{Actions::emptyA, Actions::insertFieldValueInWhere,		Actions::emptyA,						Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,			Actions::resetDataAndMoveToRequestConstruct,	Actions::resetDataAndMoveToSelectTable,		Actions::emptyA,			Actions::emptyA,		Actions::emptyA,		Actions::emptyA,		Actions::emptyA,			Actions::emptyA,			Actions::resetDataAndMoveToArchive, Actions::emptyA,			Actions::emptyA,			Actions::emptyA,		Actions::emptyA, Actions::emptyA, Actions::emptyA,},
/*12*/	{Actions::emptyA, Actions::insertLogicOperator,			Actions::emptyA,						Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,			Actions::resetDataAndMoveToRequestConstruct,	Actions::resetDataAndMoveToSelectTable,		Actions::emptyA,			Actions::emptyA,		Actions::emptyA,		Actions::emptyA,		Actions::saveSelLogicOp,	Actions::emptyA,			Actions::resetDataAndMoveToArchive, Actions::emptyA,			Actions::emptyA,			Actions::emptyA,		Actions::emptyA, Actions::emptyA, Actions::emptyA,},
/*13*/	{Actions::emptyA, Actions::addFieldToRequest,			Actions::emptyA,						Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::execRequest,		Actions::resetDataAndMoveToRequestConstruct,	Actions::resetDataAndMoveToSelectTable,		Actions::emptyA,			Actions::emptyA,		Actions::emptyA,		Actions::saveSelField,	Actions::emptyA,			Actions::emptyA,			Actions::resetDataAndMoveToArchive, Actions::emptyA,			Actions::emptyA,			Actions::emptyA,		Actions::emptyA, Actions::emptyA, Actions::emptyA,},
/*14*/	{Actions::emptyA, Actions::insertFieldValue,			Actions::emptyA,						Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,			Actions::resetDataAndMoveToRequestConstruct,	Actions::resetDataAndMoveToSelectTable,		Actions::emptyA,			Actions::emptyA,		Actions::emptyA,		Actions::emptyA,		Actions::emptyA,			Actions::emptyA,			Actions::resetDataAndMoveToArchive, Actions::emptyA,			Actions::emptyA,			Actions::emptyA,		Actions::emptyA, Actions::emptyA, Actions::emptyA,},
/*15*/	{Actions::emptyA, Actions::addFieldToRequest,			Actions::emptyA,						Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,			Actions::resetDataAndMoveToRequestConstruct,	Actions::resetDataAndMoveToSelectTable,		Actions::emptyA,			Actions::emptyA,		Actions::emptyA,		Actions::saveSelField,	Actions::emptyA,			Actions::emptyA,			Actions::resetDataAndMoveToArchive, Actions::emptyA,			Actions::emptyA,			Actions::emptyA,		Actions::emptyA, Actions::emptyA, Actions::emptyA,},
/*16_1*/{Actions::emptyA, Actions::emptyA,						Actions::emptyA,						Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,			Actions::resetDataAndMoveToRequestConstruct,	Actions::resetDataAndMoveToSelectTable,		Actions::emptyA,			Actions::emptyA,		Actions::emptyA,		Actions::emptyA,		Actions::emptyA,			Actions::emptyA,			Actions::resetDataAndMoveToArchive, Actions::moveDownSelection, Actions::moveUpSelection,	Actions::execRequest,	Actions::emptyA, Actions::closeArch, Actions::emptyA,},
/*16_2*/{Actions::emptyA, Actions::emptyA,						Actions::emptyA,						Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,			Actions::resetDataAndMoveToRequestConstruct,	Actions::resetDataAndMoveToSelectTable,		Actions::emptyA,			Actions::emptyA,		Actions::emptyA,		Actions::emptyA,		Actions::emptyA,			Actions::emptyA,			Actions::resetDataAndMoveToArchive, Actions::emptyA,			Actions::emptyA,			Actions::emptyA,		Actions::clearTable, Actions::emptyA, Actions::emptyA,},
/*f*/	{Actions::emptyA, Actions::emptyA,						Actions::emptyA,						Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,				Actions::emptyA,			Actions::emptyA,								Actions::emptyA,							Actions::emptyA,			Actions::emptyA,		Actions::emptyA,		Actions::emptyA,		Actions::emptyA,			Actions::emptyA,			Actions::emptyA,					Actions::emptyA,			Actions::emptyA,			Actions::emptyA,		Actions::emptyA, Actions::emptyA, Actions::emptyA,},

};


//0 - emptyA
//1 ok button
//2 next button
//3 select button
//4 delete button
//5 update button
//6 insert button
//7 execute button
//8 request menu item
//9 table menu item
//10 success
//11 error
//

States stateMoveTable[19][23] = {

	//			0			1			2			3			4			5			6			7			8			9			10			11			12				13			14			15														16				17			18			19			20			21				22		
	/*s*/	{States::s1, States::s,  States::s,  States::s,  States::s,  States::s,  States::s,  States::s,  States::s,  States::s,  States::s,  States::s,   States::s,   States::s,   States::s,   States::s,												States::s,   States::s,   States::s,   States::s,   States::s,   States::s,   States::s,   },
	/*1*/	{States::s1, States::s1, States::s1, States::s1, States::s1, States::s1, States::s1, States::s1, States::s6, States::s2, States::s1, States::s1,  States::s1,  States::s1,  States::s1,  States::s1,											States::s16_1,  States::s1,  States::s1,  States::s1,  States::s1,  States::s1,  States::s1,  },
	/*2*/	{States::s2, States::s2, States::s2, States::s2, States::s2, States::s2, States::s2, States::s3, States::s6, States::s2, States::s2, States::s2,  States::s2,  States::s2,  States::s2,  States::s2,											States::s16_1,  States::s2,  States::s2,  States::s2,  States::s2,  States::s2,  States::s2,  },
	/*3*/	{States::s3, States::s3, States::s3, States::s3, States::s3, States::s3, States::s3, States::s3, States::s6, States::s2, States::s5, States::s4,  States::s3,  States::s3,  States::s3,  States::s3,											States::s16_1,  States::s3,  States::s3,  States::s3,  States::s3,  States::s3,  States::s3,  },
	/*4*/	{States::s4, States::s1, States::s4, States::s4, States::s4, States::s4, States::s4, States::s4, States::s6, States::s2, States::s4, States::s4,  States::s4,  States::s4,  States::s4,  States::s4,											States::s16_1,  States::s4,  States::s4,  States::s4,  States::s4,  States::s4,  States::s4,  },
	/*5*/	{States::s5, States::s1, States::s5, States::s5, States::s5, States::s5, States::s5, States::s5, States::s6, States::s2, States::s5, States::s5,  States::s5,  States::s5,  States::s5,  States::s5,											States::s16_1,  States::s5,  States::s5,  States::s5,  States::s5,  States::s5,  States::s5,  },
	/*6*/	{States::s6, States::s6, States::s6, States::s15,States::s9, States::s7, States::s13,States::s6, States::s6, States::s2, States::s6, States::s6,  States::s6,  States::s6,  States::s6,  States::s6,											States::s16_1,  States::s6,  States::s6,  States::s6,  States::s6,  States::s6,  States::s6,  },
	/*7*/	{States::s7, States::s8, States::s9, States::s7, States::s7, States::s7, States::s7, States::s7, States::s6, States::s2, States::s7, States::s7,  States::s7,  States::s7,  States::s7,  States::s7,											States::s16_1,  States::s7,  States::s7,  States::s7,  States::s7,  States::s7,  States::s7,  },
	/*8*/	{States::s8, States::s7, States::s8, States::s8, States::s8, States::s8, States::s8, States::s8, States::s6, States::s2, States::s8, States::s8,  States::s8,  States::s8,  States::s8,  States::s8,											States::s16_1,  States::s8,  States::s8,  States::s8,  States::s8,  States::s8,  States::s8,  },
	/*9*/	{States::s9, States::s10,States::s9, States::s9, States::s9, States::s9, States::s9, States::s3, States::s6, States::s2, States::s9, States::s9,  States::s9,  States::s9,  States::s9,  States::s9,											States::s16_1,  States::s9,  States::s9,  States::s9,  States::s9,  States::s9,  States::s9,  },
	/*10*/	{States::s10,States::s11,States::s10,States::s10,States::s10,States::s10,States::s10,States::s10,States::s6, States::s2, States::s10,States::s10, States::s10, States::s10, States::s10, States::s10,											States::s16_1, States::s10, States::s10, States::s10, States::s10, States::s10, States::s10, },
	/*11*/	{States::s11,States::s12,States::s11,States::s11,States::s11,States::s11,States::s11,States::s11,States::s6, States::s2, States::s11,States::s11, States::s11, States::s11, States::s11, States::s11,											States::s16_1, States::s11, States::s11, States::s11, States::s11, States::s11, States::s11, },
	/*12*/	{States::s12,States::s9, States::s12,States::s12,States::s12,States::s12,States::s12,States::s12,States::s6, States::s2, States::s12,States::s12, States::s12, States::s12, States::s12, States::s12,											States::s16_1, States::s12, States::s12, States::s12, States::s12, States::s12, States::s12, },
	/*13*/	{States::s13,States::s14,States::s13,States::s13,States::s13,States::s13,States::s13,States::s3, States::s6, States::s2, States::s13,States::s13, States::s13, States::s13, States::s13, States::s13,											States::s16_1, States::s13, States::s13, States::s13, States::s13, States::s13, States::s13, },
	/*14*/	{States::s14,States::s13,States::s14,States::s14,States::s14,States::s14,States::s14,States::s14,States::s6, States::s2, States::s14,States::s14, States::s14, States::s14, States::s14, States::s14,											States::s16_1, States::s14, States::s14, States::s14, States::s14, States::s14, States::s14, },
	/*15*/	{States::s15,States::s15,States::s9, States::s15,States::s15,States::s15,States::s15,States::s15,States::s6, States::s2, States::s15,States::s15, States::s15, States::s15, States::s15, States::s15,											States::s16_1, States::s15, States::s15, States::s15, States::s15, States::s15, States::s15, },
	/*f*/	{States::s16_1,States::s16_1,States::s16_1,States::s16_1,States::s16_1,States::s16_1,States::s16_1,States::s16_1,States::s16_1,States::s16_1,States::s16_1,States::s16_1,States::s16_1,States::s16_1,States::s16_1,States::s16_1,				States::s16_1,States::s16_1,States::s16_1,States::s16_2,States::s16_1,States::s1,States::s16_1,},
	/*f*/	{States::s16_2,States::s16_2,States::s16_2,States::s16_2,States::s16_2,States::s16_2,States::s16_2,States::s16_2,States::s16_2,States::s16_2,States::s16_2,States::s16_2,States::s16_2,States::s16_2,States::s16_2,States::s16_2,				States::s16_1,States::s16_2,States::s16_2,States::s16_2,States::s16_1,States::s16_2,States::s16_2,},
	/*f*/	{States::f,  States::f,  States::f,  States::f,  States::f,  States::f,  States::f,  States::f,  States::f,  States::f,  States::f,  States::f,   States::f,   States::f,   States::f,   States::f,   States::f,   States::f,   States::f,   States::f,   States::f,   States::f,   States::f, },

};


