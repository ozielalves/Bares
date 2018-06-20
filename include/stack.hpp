/**
 * @file stack.hpp
 * @version 1.0
 * @date May, 14.
 * @author Daniel Guerra and Oziel Alves
 * @title Stack
 * @brief Stack class to handle the operations
 */

#include <iostream>
#include <stdexcept>

namespace sc
{	
	template <typename T>
 
	class stack{
		
		private:

		T *storage; //!< Store the data
		size_t size_; //!< Capacity size_
		size_t top_; //!< top_ index
		
		/*! @brief Double the store capacity. */
		void double_storage(void){
			
			T* temp = new T[2*size_]; // Reserves a doubled store capacity into a temp ptr
			
			for( auto i(0u); i < top_; i++) // Copy the data to the new  area
			{
				temp[i] = storage[i];
			}
			
			delete [] storage; // Free the old one

			
			storage = temp; // Redirecting the pointer

			
			size_ *= 2; 

		}
		public:
			/*! @brief Constructor. */
			stack(void) : storage(new T[1]), size_(1), top_(0){} // Initializer list
			
			/*! @brief Destructor. */
			~stack(void){

				delete [] storage;
			}
	

		/*! @brief Inserts an element into the stack. */
		void push(const T& value){  
			
			// If is full double the storage capacity
			if(size_ == top_)	double_storage();
			
			storage[top_++] = value;		
		}

		/*! @brief Removes the first element from the stack. */
		void pop( ){ 

			if( empty() )
				throw std::runtime_error("You can't access an empty stack!");

			--top_;
		}

		/*! @return The element at the top of the stack. */
		T top( ) const {
			
			if( empty() )
				throw std::runtime_error("You can't access an empty stack!");

			return storage[top_-1];
		}

		/*! @brief Determines if the stack is empty. */
		bool empty(void ) const {
		
			return top_ == 0;
		}

		/*! @return The stack size_. */
		size_t size(void ) const{
			
			return top_;
		}

		/*! @brief Clear the stack. */
		void clear(void ){
			
			top_ = 0;
		}
	};
}	
