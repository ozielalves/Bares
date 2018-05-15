/**
 * @file stack.cpp
 * @version 1.0
 * @date May, 14.
 * @author Daniel Guerra and Oziel Alves
 * @title Stack
 * @brief Stack class to handle the operations
 */

#include <iostream>

namespace sc
{	
	template <typename T>
 
	class stack{
		
		private:

		size_t size; //!< Capacity size
		size_t top; //!< Top index
		T *storage; //!< Store the data

		/*! @brief Double the store capacity. */
		void double_storage(void){
			
			T* temp = new [2*size]; // Reserves a doubled store capacity into a temp ptr
			
			for( auto(0u); i < top; i++) // Copy the data to the new  area
			{
				temp[i] = size[i];
			}
			
			delete [] storage; // Free the old one

			
			storage = temp; // Redirecting the pointer

			
			size *= 2; 

		}
		public:
			/*! @brief Constructor. */
			stack(void) : size(new T[1]), size(1), top(0){} // Initializer list
			
			/*! @brief Destructor. */
			~stack(void){

				delete [] storage;
			}
	

		/*! @brief Inserts an element into the stack. */
		void push(const T& value){  
			
			// If is full double the storage capacity
			if(size == top)	double_storage();
			
			storage[top++] = value;		
		}

		/*! @brief Removes the first element from the stack. */
		void pop(){ 

			if( empty() )
				throw std::runtime_error("You can't access an empty stack!");

			--top;
		}

		/*! @return The element at the top of the stack. */
		T top(void ) const {
			
			if( empty() )
				throw std::runtime_error("You can't access an empty stack!");

			return storage[top-1];
		}

		/*! @brief Determines if the stack is empty. */
		bool empty(void ) const {
		
			return top == 0;
		}

		/*! @return The stack size. */
		size_t size(void ) const{
			
			return top;
		}

		/*! @brief Clear the stack. */
		void clear(void ){
			
			top = 0;
		}
	};
}	