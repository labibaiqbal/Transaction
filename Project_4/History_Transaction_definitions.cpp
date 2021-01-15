#ifndef _HISTORY_TRANSACTION_CPP_
#define _HISTORY_TRANSACTION_CPP_

#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK 1
//
Transaction::Transaction( std::string ticker_symbol, unsigned int day_date, unsigned int month_date, unsigned year_date,
		bool buy_sell_trans, unsigned int number_shares, double trans_amount ){
	trans_id= assigned_trans_id++;
	day=day_date;
	month=month_date;
	year=year_date;
	symbol=ticker_symbol;
	if(buy_sell_trans){
		trans_type="Buy";
	}else{
		trans_type="Sell";
	}
	shares=number_shares;
	amount=trans_amount;
	p_next=nullptr;
	acb=0.0;
	acb_per_share=0.0;
	cgl=0.0;
	share_balance=0.0;
}


// Destructor
// TASK 1
//
Transaction::~Transaction(){}


// Overloaded < operator.
// TASK 2
//


bool Transaction::operator<( Transaction const &other ){

	if(this->year<other.year){
		return true;

	}else if(this->year==other.year){
		if(this->month<other.month){
			return true;
		}else if(this->month==other.month){
			if(this->day<other.day){
				return true;
			}else{
				return false;
			}
		}else{
			return false;
		}
	}else{
		return false;
	}

	return false;

}







// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK 3
//
History::History(){
	p_head=nullptr;
}


// Destructor
// TASK 3
//

History::~History(){
	Transaction *p_temp{nullptr};
	while(p_head!=nullptr){
		p_temp=p_head;
		p_head=p_head->get_next();
		delete p_temp;
	}
	p_head=nullptr;



}


// read_transaction(...): Read the transaction history from file.
// TASK 4
//;
void History::read_history(){

	ece150::open_file();

	 while(ece150::next_trans_entry()){
		 Transaction *p_transaction = new Transaction(
			ece150::get_trans_symbol(),
			 ece150::get_trans_day(),
			 ece150::get_trans_month(),
			 ece150::get_trans_year(),
			 ece150::get_trans_type(),
			 ece150::get_trans_shares(),
			 ece150::get_trans_amount()
		 );
		 insert(p_transaction);
	 }

	ece150::close_file();

}








// insert(...): Insert transaction into linked list.
// TASK 5
//

void History::insert( Transaction *p_new_trans ){
	if(p_head==nullptr){
		p_head=p_new_trans;
	}
	else{
		Transaction *p_traverse{p_head};
		while(p_traverse->get_next()!=nullptr){
			p_traverse=p_traverse->get_next();


		}
	p_traverse->set_next(p_new_trans);

	}


}





// sort_by_date(): Sort the linked list by trade date.
// TASK 6
//
void History::sort_by_date(){

	if(p_head==nullptr||p_head->get_next()==nullptr){
		return;
	}
	Transaction *p_sorted=nullptr;
	Transaction *p_temp1=p_head;
	Transaction *p_temp2=nullptr;
	p_head=p_head->get_next();
	p_temp1->set_next(nullptr);
	p_sorted=p_temp1;
	while(p_head!=nullptr){
		p_temp1=p_head;
		p_head=p_head->get_next();
		p_temp1->set_next(nullptr);
		if(*p_temp1<*p_sorted){
			p_temp1->set_next(p_sorted);
			p_sorted=p_temp1;

		}else{
			p_temp2=p_sorted;
			while(p_temp2->get_next()!=nullptr&&!(*p_temp1<*(p_temp2->get_next()))){
				p_temp2=p_temp2->get_next();
			}
			p_temp1->set_next(p_temp2->get_next());
			p_temp2->set_next(p_temp1);

		}

	}
	p_head=p_sorted;






}





// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//

void History::update_acb_cgl(){



	Transaction *p_first_trans=p_head;
	Transaction *p_trans1=nullptr;
	Transaction *p_trans2=nullptr;
	if(p_first_trans!=nullptr){

		p_first_trans->set_acb(p_first_trans->get_amount());
		p_first_trans->set_share_balance(p_first_trans->get_shares());
		p_first_trans->set_acb_per_share(p_first_trans->get_acb()/p_first_trans->get_share_balance());
		p_trans1=p_first_trans;
		p_trans2=p_first_trans->get_next();
	}
		while(p_trans2!=nullptr){
			if(p_trans2->get_trans_type()){
				p_trans2->set_acb(p_trans1->get_acb()+p_trans2->get_amount());
				p_trans2->set_share_balance(p_trans1->get_share_balance()+p_trans2->get_shares());
				p_trans2->set_acb_per_share(p_trans1->get_acb()/p_trans2->get_share_balance());

			}else{
				p_trans2->set_acb(p_trans1->get_acb()-(p_trans2->get_shares()*p_trans1->get_acb_per_share()));
				p_trans2->set_share_balance(p_trans1->get_share_balance() - p_trans2->get_shares());
				p_trans2->set_acb_per_share(p_trans2->get_acb()/p_trans2->get_share_balance());
				p_trans2->set_cgl(p_trans2->get_amount()-((p_trans2->get_shares()*p_trans1->get_acb_per_share())));

			}
			p_trans1=p_trans2;
			p_trans2=p_trans2->get_next();


		}




}





// compute_cgl(): )Compute the ACB, and CGL.
// TASK 8

double History::compute_cgl( unsigned int year ){

	Transaction *p_cgl=p_head;
	double total_cgl=0.0;
	while(p_cgl != nullptr){
		if( p_cgl->get_year()==year){
			total_cgl=total_cgl + p_cgl->get_cgl();
		}
		p_cgl=p_cgl->get_next();
	}
	return total_cgl;



}




// print() Print the transaction history.
//TASK 9
//
void History::print(){
	Transaction *p_temp=p_head;
	std::cout<<"============ BEGIN TRANSACTION HISTORY ============"<<std::endl;
	while(p_temp!=nullptr){
		p_temp->print();
		p_temp=p_temp->get_next();

	}
	std::cout<<"============ END TRANSACTION HISTORY ============"<<std::endl;




}



// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }


#endif
