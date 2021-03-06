#include "ast.h"
#include "structures.h"
#include <typeinfo>


using namespace AST;

#define print std::cout
#define _case(X) {X break;}
#define _casePrint(X) {print << "X"; break;}



bool BinOp::isArithmetic() {
	switch( op ) {
		case Operation::oplus:;
		case Operation::ominus:;
		case Operation::omult:;
		case Operation::odiv: {return true;}
		default: {return false;}
	}
}

bool BinOp::isBinary() {
	switch( op ) {
		case Operation::oand:;
		case Operation::oor: {return true;}
		default: {return false;}
	}
}

bool BinOp::isComparation() {
	switch( op ) {
		case Operation::oequal:;
		case Operation::ogreater:;
		case Operation::oless:;
		case Operation::ogreatereq:;
		case Operation::olesseq:;
		case Operation::odifferent: {return true;}
		default: {return false;}
	}
}

bool BinOp::isAttribution() {
	return op == Operation::oassign;
}

void BinOp::verifyOperands() {
	if( isArithmetic() ) {
		switch( left->type ) {
			case AST::tInt: {
				switch ( right->type ) {
					case AST::tInt:_case( this->type = AST::tInt; )
					case AST::tReal: _case( left->coercionTo=AST::tReal; this->type = AST::tReal; )
					default:_case( this->type = AST::tInt; yyerror( "Erro semantico: operacao espera %s ou %s mas recebeu %s.\n", tipoOperacoes[( int )AST::tInt].c_str(), tipoOperacoes[( int )AST::tReal].c_str(), tipoOperacoes[( int )right->type].c_str() ); ); //TODO Caso não seja compativel com INT!
				}
				break;
			}
			case AST::tReal: {
				this->type=AST::tReal;
				switch ( right->type ) {
					case AST::tInt: _case( right->coercionTo=AST::tReal; )
					case AST::tBool: _case( yyerror( "Erro semantico: operacao espera %s ou %s mas recebeu %s.\n", tipoOperacoes[( int )AST::tInt].c_str(), tipoOperacoes[( int )AST::tReal].c_str(), tipoOperacoes[( int )right->type].c_str() );  ); //TODO Caso não seja compativel com REAL!
				}
				break;
			}
			case AST::tBool: {
				this->type=AST::tInt;
				yyerror( "Erro semantico: operacao espera %s ou %s mas recebeu %s.\n", tipoOperacoes[( int )AST::tInt].c_str(), tipoOperacoes[( int )AST::tReal].c_str(), tipoOperacoes[( int )left->type].c_str() );
				break;
			}
		}
	} else if( isComparation() ) {
		this->type = AST::tBool;
		switch( left->type ) {
			case AST::tInt: {
				switch ( right->type ) {
					case AST::tReal: _case( left->coercionTo=AST::tReal; )
					case AST::tBool:_case(  yyerror( "Erro semantico: operacao espera %s ou %s mas recebeu %s.\n", tipoOperacoes[( int )AST::tInt].c_str(), tipoOperacoes[( int )AST::tReal].c_str(), tipoOperacoes[( int )right->type].c_str() ); ); //TODO Caso não seja compativel com INT!
				}
				break;
			}
			case AST::tReal: {
				this->type=AST::tReal;
				switch ( right->type ) {
					case AST::tInt: _case( right->coercionTo=AST::tReal; )
					case AST::tBool: _case( yyerror( "Erro semantico: operacao espera %s ou %s mas recebeu %s.\n", tipoOperacoes[( int )AST::tInt].c_str(), tipoOperacoes[( int )AST::tReal].c_str(), tipoOperacoes[( int )right->type].c_str() );  ); //TODO Caso não seja compativel com REAL!
				}
				break;
			}
			case AST::tBool: {
				if( right->type != AST::tBool ) {
					yyerror( "Erro semantico: operacao espera %s mas recebeu %s.\n", tipoOperacoes[( int ) left->type].c_str(), tipoOperacoes[( int ) right->type].c_str() );
				}
				break;
			}
		}
	} else if( isBinary() ) {
		this->type = AST::tBool;
		if( left->type != this->type ) {
			yyerror( "Erro semantico: operacao espera %s mas recebeu %s.\n", tipoOperacoes[( int ) this->type].c_str(), tipoOperacoes[( int ) left->type].c_str() );
		} else if( right->type != this->type ) {
			yyerror( "Erro semantico: operacao espera %s mas recebeu %s.\n", tipoOperacoes[( int ) this->type].c_str(), tipoOperacoes[( int ) right->type].c_str() );
		}
	} else if( isAttribution() ) {
		if( left->type != right->type ) {
			if( !( ( left->type == AST::tReal ) && ( right->type == AST::tInt ) ) ) {
				yyerror( "Erro semantico: operacao %s espera %s mas recebeu %s.\n",printOp().c_str(), tipoOperacoes[( int ) left->type].c_str(), tipoOperacoes[( int ) right->type].c_str() );
			} else {
				right->coercionTo=AST::tReal;
			}
		}
	}
}

std::string BinOp::printOp() {
	switch( op ) {
		case AST::oplus: {return "soma";}
		case AST::omult: {return"multiplicacao";}
		case AST::oassign: { return "atribuicao";}
		case AST::ominus: {return "subtracao";}
		case AST::oand: {return "e";}
		case AST::odifferent: {return "diferente";}
		case AST::odiv: {return "divisao";}
		case AST::oequal: {return "igual";}
		case AST::ogreater: {return "maior";}
		case AST::ogreatereq: {return "maior ou igual";}
		case AST::oless: {return "menor";}
		case AST::olesseq: {return "menor ou igual";}
		case AST::oor: {return "ou";}
		case AST::onot: {return "not";}
	}
}
//===============================================


/* ================Print Methods================*/
//Integer
//===============================================

void Integer::printTree() {
	if( hasParentheses ) {
		std::cout<< "(abre parenteses) ";
	}
	std::cout << "inteiro "<< value;
	if( this->coercionTo ) {
		std::cout << " para variavel " <<  tipoOperacoes[( int )this->coercionTo].c_str();
	}
	if( hasParentheses ) {
		std::cout<< " (fecha parenteses)";
	}
	return;
}

//Double
//===============================================

void Double::printTree() {
	if( hasParentheses ) {
		std::cout<< "(abre parenteses) " << "real "<<value << " (fecha parenteses)";
	} else {
		std::cout << "real "<<value;
	}
}
//Boolean
//===============================================
void Boolean::printTree() {
	std::string result = ( value?"TRUE":"FALSE" );
	if( hasParentheses ) {
		std::cout<< "(abre parenteses) " << "booleano "<<result << " (fecha parenteses)";
	} else {
		std::cout << "booleano "<<result;
	}
	return;
}


//Binary Operation
//===============================================
void BinOp::printTree() {
	bool isAss = op == AST::oassign;
	if( !isAss ) {
		print << "(";
	}
	if( hasParentheses ) {
		std::cout<< "(abre parenteses) ";
	}
	left->printTree();
	switch( op ) {
		case AST::oplus: _case( print << " ("<<printOp() << " " << tipoOperacoes[( int )this->type] << ") "; )
		case AST::omult: _case( print << " ("<<printOp() << " " << tipoOperacoes[( int )this->type] << ") "; )
		case AST::oassign: _case( print<< " "; )
		case AST::ominus: _case( print << " ("<<printOp() << " " << tipoOperacoes[( int )this->type] << ") "; )
		case AST::oand: _case( print << " ("<<printOp() << " " << tipoOperacoes[( int )this->type] << ") "; )
		case AST::odifferent: _case( print << " ("<<printOp()<< tipoOperacoes[( int )this->type] << ") "; )
		case AST::odiv: _case( print << " ("<<printOp() << " " << tipoOperacoes[( int )this->type] << ") "; )
		case AST::oequal: _case ( print << " ("<<printOp() << " " << tipoOperacoes[( int )this->type] << ") "; )
		case AST::ogreater: _case ( print << " ("<<printOp() << " " << tipoOperacoes[( int )this->type] << ") "; )
		case AST::ogreatereq: _case ( print << " ("<<printOp() << " " << tipoOperacoes[( int )this->type] << ") "; )
		case AST::oless:_case( print << " ("<<printOp() << " " << tipoOperacoes[( int )this->type] << ") "; )
		case AST::olesseq: _case ( print << " ("<<printOp() << " " << tipoOperacoes[( int )this->type] << ") "; )
		case AST::oor:_case ( print << " ("<<printOp() << " " << tipoOperacoes[( int )this->type] << ") "; )
	}
	right->printTree();
	if( hasParentheses ) {
		std::cout<< " (fecha parenteses)";
	}
	if( !isAss ) {
		print << ")";
	}
	return;
}
//Block
//===============================================
void Block::printTree() {
	for ( Node *line: lines ) {
		line->printTree();
		std::cout << std::endl;
	}
}
//Variable
//===============================================

void Variable::printTree() {
	if( next != nullptr ) {
		next->printTree();
		std::cout << ", ";
	} else {
		// std::string message = "Tipo Fantasma " + this->id;
		switch ( this->useType ) {
			case AST::Variable::atrib: {
				if( idType=="" ) {
					std::cout << "Atribuicao de valor para variavel " + TypesString[( int )type] +" " + this->id +":";
				} else {
					AST::Variable *castToVar = dynamic_cast<AST::Variable *>( component );
					std::string componentName;
					AST::Types  componentType;
					if( castToVar != nullptr ) {
						componentName = castToVar->id;
						componentType = castToVar->type;
						std::cout << "Atribuicao de valor para variavel " + idType +" " + this->id +" componente " + TypesString[( int )componentType] +" "+ componentName +":";
					} else {
						AST::ArrayItem *castToArray = dynamic_cast<AST::ArrayItem *>( component );
						componentName = castToArray->id;
						componentType = castToArray->type;
						AST::Node *index = castToArray->index;
						std::cout << "Atribuicao de valor para variavel " + idType +" " + this->id +" componente arranjo " + TypesString[( int )componentType] +" "+ componentName +"{+indice: ";
						index->printTree();
						std::cout<<"}: ";
					}
				}
				break;
			}
			case AST::Variable::ini: {
				if( idType=="" ) {
					std::cout << "Declaracao de variavel " + TypesString[( int )type] +": "+ this->id ;
				} else {
					std::cout << "Declaracao de variavel " + idType +": "+ this->id ;
				}
				break;
			}
			case AST::Variable::read: {
				//std::cout<<"[ST  com "<<simbolTable->symbolMap.size()<<" atribuiu? "<<simbolTable->jaAtribuiu<<"]";
				switch( simbolTable->getidentifierType( this->id ) ) {
					case Structures::tBool: {
						if( hasParentheses ) {
							std::cout << "(abre parenteses) booleano "+ this->id +" (fecha parenteses)";
						} else {
							std::cout << "booleano " + this->id;
						}
						break;
					}
					case Structures::tDouble: {
						if( hasParentheses ) {
							std::cout << "(abre parenteses) real "+ this->id +" (fecha parenteses)";
						} else {
							std::cout << "real " + this->id;
						}
						break;
					}
					case Structures::tInteger: {
						// message = "";
						if( hasParentheses ) {
							std::cout << "(abre parenteses) ";
						}
						std::cout << "inteiro " + this->id;
						if( this->coercionTo ) {
							std::cout << ( " para variavel " + tipoOperacoes[( int )this->coercionTo] );
						}
						if( hasParentheses ) {
							std::cout << " (fecha parenteses)";
						}
						break;
					}
					case Structures::compound: {
						AST::Variable *castToVar = dynamic_cast<AST::Variable *>( component );
						std::string componentName;
						AST::Types  componentType;
						AST::Node *index;
						if( castToVar != nullptr ) {
							componentName = castToVar->id;
							componentType = castToVar->type;
							if( hasParentheses ) {
								std::cout << "(abre parenteses) "+idType +" " + this->id +" componente " + TypesString[( int )componentType] +" "+ componentName +" (fecha parenteses)";
							} else {
								std::cout << idType +" " + this->id +" componente " + TypesString[( int )componentType] +" "+ componentName;
							}
						} else {
							AST::ArrayItem *castToArray = dynamic_cast<AST::ArrayItem *>( component );
							componentName = castToArray->id;
							componentType = castToArray->type;
							index = castToArray->index;
							if( hasParentheses ) {
								std::cout << "(abre parenteses) "+idType +" " + this->id +" componente arranjo " + TypesString[( int )componentType] +" "+ componentName +": {+indice: ";
								index->printTree();
								std::cout<<"} (fecha parenteses)";
							} else {
								std::cout << idType +" " + this->id +" componente arranjo " + TypesString[( int )componentType] +" "+ componentName +": {+indice: ";
								index->printTree();
								std::cout<<"}";
							}
						}
						break;
					}
					case Structures::undefined: {
						if( hasParentheses ) {
							std::cout << "(abre parenteses) indefinido "+ this->id +" (fecha parenteses)";
						} else {
							std::cout << "indefinido " + this->id;
						}
						break;
					}
				};
				//message="leitura de variavel "
				//message = "";  break;
			}
		}
		// std::cout<<message;
		return;
	}
	std::cout << this->id;
	return;
}
void Array::printTree() {
	if( next != nullptr ) {
		next->printTree();
		std::cout << ", ";
	} else {
		std::string message = "";
		switch ( this->useType ) {
			case AST::Array::ini: {
				if( idType=="" ) {
					message = "Declaracao de arranjo " + TypesString[( int )type] + " de tamanho "+std::to_string( size ) + ": "+id;
				} else {
					message = "Declaracao de arranjo " + idType + " de tamanho "+std::to_string( size ) + ": "+id;
				}
				break;
			}
		}
		std::cout<<message;
		return;
	}
	std::cout << this->id;
	return;
}
void ArrayItem::printTree() {
	std::string message = "";
	switch ( this->useType ) {
		case AST::ArrayItem::atrib: {
			message = "Atribuicao de valor para arranjo "+ TypesString[( int )type] + " " + this->id + ": {+indice: " ;
			std::cout<<message;
			this->index->printTree();
			std::cout<<"}: ";
			break;
		}
		case AST::ArrayItem::read: {
			if( idType=="" ) {
				message = "arranjo "+ TypesString[( int )type] + " " + this->id + ": {+indice: " ;
				std::cout<<message;
				this->index->printTree();
				std::cout<<"}";
			} else {
				AST::Variable *castToVar = dynamic_cast<AST::Variable *>( component );
				std::string componentName;
				AST::Types  componentType;
				if( castToVar != nullptr ) {
					componentName = castToVar->id;
					componentType = castToVar->type;
					std::cout << "arranjo " + idType +" " + this->id +" componente " + TypesString[( int )componentType] +" "+ componentName +": {+indice: ";
					this->index->printTree();
					std::cout<<"}";
				} else {
					AST::ArrayItem *castToArray = dynamic_cast<AST::ArrayItem *>( component );
					componentName = castToArray->id;
					componentType = castToArray->type;
					AST::Node *indexComp = castToArray->index;
					std::cout << "arranjo " + idType +" " + this->id +" componente " + TypesString[( int )componentType] +" {+indice: ";
					this->index->printTree();
					std::cout<<"}: ";
					std::cout << " "+ componentName +": {+indice: ";
					indexComp->printTree();
				}
			}
			break;
		}
		return;
	}
}

void Function::printTree() {
	switch ( useType ) {
		case AST::Function::decl: {
			std::cout << "Declaracao de funcao " << AST::tipoOperacoes[( int )this->type] << std::endl;
			this->printParams( params );
			std::cout <<"\nFim Declaracao";
			break;
		}
		case AST::Function::def: {
			std::cout << "Definicao de funcao " << AST::tipoOperacoes[(int)this->type] << std::endl;
			this->printParams(params);
			std::cout << "\n+Corpo\n";
			if(this->corpo != nullptr){
				corpo->printTree();
			}
		} //Tem que printar o corpo da funcao
	};
}

void ArrayItem::verifyIndex() {
	if( index->type!=AST::tInt ) {
		yyerror( "Erro semantico: indice de tipo %s.\n",tipoOperacoes[( int )index->type].c_str() );
	}
}
void UnaryOp::printTree() {
	if( coercionTo ) {
		operand->coercionTo = coercionTo;
	}
	switch( op ) {
		case AST::ominus: {
			print << "((menos unario) ";
			operand->printTree();
			print << ")";
			break;
		}
		case AST::onot: {
			print << "((nao) ";
			operand->printTree();
			print << ")";
			break;
		}
	}
}
void Loop::printTree() {
	std::cout <<"Laco\n";
	std::cout <<"+enquanto:";
	expr->printTree();
	std::cout <<"\n+faca:\n";
	stmts->printTree();
	std::cout <<"Fim laco\n";
}
void Loop::verifyExpression() {
	if( expr->type != AST::tBool ) {
		yyerror( "Erro semantico: operacao teste espera booleano mas recebeu %s.\n",tipoOperacoes[( int )expr->type].c_str() );
	}
}
void Conditional::printTree() {
	std::cout <<"Expressao condicional\n";
	std::cout <<"+se:";
	expr->printTree();
	std::cout <<"\n+entao:\n";
	ifblock->printTree();
	if( elseblock != NULL ) {
		std::cout <<"+senao:\n";
		elseblock->printTree();
	}
	std::cout <<"Fim expressao condicional\n";
}
void Conditional::verifyExpression() {
	if( expr->type != AST::tBool ) {
		yyerror( "Erro semantico: operacao teste espera booleano mas recebeu %s.\n",tipoOperacoes[( int )expr->type].c_str() );
	}
}

void Compound::printTree() {
	std::cout << "Definicao tipo: " << id << "\n+componentes:\n";
	for ( Node *decl: components->lines ) {
		AST::Variable *castToVar = dynamic_cast<AST::Variable *>( decl );
		if( castToVar != nullptr ) {
			std::cout << "Componente " << tipoOperacoes[( int )castToVar->type] <<": " << castToVar->id <<"\n";
		} else {
			AST::Array *castToArray = dynamic_cast<AST::Array *>( decl );
			std::cout << "Componente arranjo " << tipoOperacoes[( int )castToArray->type] <<" de tamanho "<<castToArray->size <<": " << castToArray->id <<"\n";
		}
	}
	std::cout << "Fim definicao";
}








