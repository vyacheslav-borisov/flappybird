#pragma once

#include <exception>

namespace pegas
{
		#define SmartPointer ptr
		#define smart_pointer ptr
		#define smart_ptr ptr
		#define shared_ptr ptr
		
		struct NullPointerException : public std::exception
		{
			NullPointerException() throw() {}
			~NullPointerException() throw() {}
	
			const char* what() const throw()
			{
				return "[SmartPointer Exception] Attempted to dereference null pointer";
			}
		};

		struct Counter 
		{
			Counter(unsigned c = 1) : count(c) {}
			unsigned count; 
		};

		template <typename X> 
		class ptr
		{

		public:
			typedef X element_type;

		/* 
			ptr needs to be its own friend so ptr< X > and ptr< Y > can access
			each other's private data members 
		*/ 
		template <class Y> friend class ptr; 
		/* 
			default constructor
			- don't create Counter
		*/
		ptr() : rawPtr(0), counter(0) { }
	
		/*
			Construct from a raw pointer 
		*/
		ptr(X* raw, Counter* c = 0) : rawPtr(0), counter(0)
		{
			if (raw)
			{
				rawPtr = raw; 
				if (c) acquire(c);
				else counter = new Counter;
			}
		}
	
		template <typename Y>
 		explicit ptr(Y* raw, Counter* c = 0) : rawPtr(0), counter(0)
		{
			if (raw)
			{
				rawPtr = static_cast<X*>( raw );
				if (c) acquire(c);
				else counter = new Counter; 
			}
		}
	
	
		/*
			Copy constructor 
		*/
		ptr(const ptr< X >& otherPtr)
		{
			acquire( otherPtr.counter );
			rawPtr = otherPtr.rawPtr;
		}
	
		template <typename Y>
		explicit ptr(const ptr< Y >& otherPtr) : rawPtr(0), counter(0)
		{
			acquire(otherPtr.counter);
			rawPtr = static_cast<X*>( otherPtr.GetRawPointer());
		}
	

		/* 
			Destructor 
		*/ 
		~ptr()
		{
			release();
		}

		/*
			Assignment to another ptr 
		*/

		ptr& operator=(const ptr< X >& otherPtr)
		{
			if (this != &otherPtr)
			{
				release();
				acquire(otherPtr.counter); 
				rawPtr = otherPtr.rawPtr; 
			}
			return *this; 
		}

		template <typename Y>
		ptr& operator=(const ptr< Y >& otherPtr)
		{
			if ( this != (ptr< X >*) &otherPtr )
			{
				release();
				acquire(otherPtr.counter);
				rawPtr = static_cast<X*> (otherPtr.GetRawPointer());
			}
			return *this;
		}

		/*
			Assignment to raw pointers is really dangerous business.
			If the raw pointer is also being used elsewhere,
			we might prematurely delete it, causing much pain.
			Use sparingly/with caution.
		*/

		ptr& operator=(X* raw)
		{

			if (raw)
			{
				release(); 
				counter = new Counter; 
				rawPtr = raw; 
			}
			return *this;
		}

		template <typename Y>
		ptr& operator=(Y* raw)
		{
			if (raw)
			{
				release();
				counter = new Counter; 
				rawPtr = static_cast<X*>(raw);
			}
			return *this;
		}

		/* 
			assignment to long to allow ptr< X > = NULL, 
			also allows raw pointer assignment by conversion. 
			Raw pointer assignment is really dangerous!
			If the raw pointer is being used elsewhere, 
			it will get deleted prematurely. 
		*/ 
		ptr& operator=(long num)
		{
			if (num == 0)  //pointer set to null
			{
				release(); 
			}

			else //assign raw pointer by conversion
			{
				release();
				counter = new Counter; 
				rawPtr = reinterpret_cast<X*>(num);
			}	

			return *this; 
		} 

		/*
			Member Access
		*/
		X* operator->() const 
		{
			return GetRawPointer(); 
		}


		/*
			Dereference the pointer
		*/
		X& operator* () const 
		{
			return *GetRawPointer(); 
		}


		/*
			Conversion/casting operators
		*/


		operator bool() const
		{
			return IsValid();
		}

	
		/*
			implicit casts to base types of the
			the pointer we're storing 
		*/
	
		template <typename Y>
		operator Y*() const
		{
			return static_cast<Y*>(rawPtr);  
		}

		template <typename Y>
		operator const Y*() const
		{
			return static_cast<const Y*>(rawPtr);
		}

		template <typename Y>
		operator ptr<Y>()
		{
			//new ptr must also take our counter or else the reference counts 
			//will go out of sync
			return ptr<Y>(rawPtr, counter);
		}


		/*
			Provide access to the raw pointer 
		*/

		X* GetRawPointer() const         
		{
			if (rawPtr == 0) throw new NullPointerException;
			return rawPtr;
		}

		/*
			Для обеспечения совместимости с уже написаным кодом используюшим семантику 
			boost::shared_ptr:
		*/
		X* get() const
		{
			return GetRawPointer();
		}

	
		/* 
			Is there only one reference on the counter?
		*/
		bool IsUnique() const
		{
			if (counter && counter->count == 1) return true; 
			return false; 
		}
	
		bool IsValid() const
		{
			if (counter && rawPtr) return true;
			return false; 
		}

		unsigned GetCount() const
		{
			if (counter) return counter->count;
			return 0;
		}

	private:
		X* rawPtr;

		Counter* counter;

		// increment the count
		void acquire(Counter* c) 
		{ 
 			counter = c;
			if (c)
			{
				(c->count)++;
			}
		}

		// decrement the count, delete if it is 0
		void release()
		{ 
			if (counter) 
			{			
				(counter->count)--; 	

				if (counter->count == 0) 
				{
					delete counter;			
					delete rawPtr;
				}
			}
			counter = 0;
			rawPtr = 0; 
		}
	};


	template <typename X, typename Y>
	bool operator==(const ptr< X >& lptr, const ptr< Y >& rptr) 
	{
		return lptr.GetRawPointer() == rptr.GetRawPointer(); 
	}

	template <typename X, typename Y>
	bool operator==(const ptr< X >& lptr, Y* raw) 
	{
		return lptr.GetRawPointer() == raw ; 
	}

	template <typename X>
	bool operator==(const ptr< X >& lptr, long num)
	{
		if (num == 0 && !lptr.IsValid())  //both pointer and address are null
		{
			return true; 
		}

		else //convert num to a pointer, compare addresses
		{
			return lptr == reinterpret_cast<X*>(num);
		}
	 
	} 

	template <typename X, typename Y>
	bool operator!=(const ptr< X >& lptr, const ptr< Y >& rptr) 
	{
		return ( !operator==(lptr, rptr) );
	}

	template <typename X, typename Y>
	bool operator!=(const ptr< X >& lptr, Y* raw)	
	{
		return ( !operator==(lptr, raw) );
	}

	template <typename X>
	bool operator!=(const ptr< X >& lptr, long num)
	{
		return (!operator==(lptr, num) ); 
	}

	template <typename X, typename Y>
	bool operator&&(const ptr< X >& lptr, const ptr< Y >& rptr)
	{
		return lptr.IsValid() &&  rptr.IsValid();
	}

	template <typename X>
	bool operator&&(const ptr< X >& lptr, bool rval)
	{
		return lptr.IsValid() && rval;
	}

	template <typename X>
	bool operator&&(bool lval, const ptr< X >& rptr)
	{
		return lval &&  rptr.IsValid();
	}

	template <typename X, typename Y>
	bool operator||(const ptr< X >& lptr, const ptr< Y >& rptr)
	{
		return lptr.IsValid() || rptr.IsValid();
	}

	template <typename X>
	bool operator||(const ptr< X >& lptr, bool rval)
	{
		return lptr.IsValid() || rval;
	}

	template <typename X>
	bool operator||(bool lval, const ptr< X >& rptr)
	{
		return lval || rptr.IsValid(); 
	}

	template <typename X>
	bool operator!(const ptr< X >& p)
	{
		return (!p.IsValid());
	}


	/* less than comparisons for storage in containers */
	template <typename X, typename Y>
	bool operator< (const ptr< X >& lptr, const ptr < Y >& rptr)
	{
		return lptr.GetRawPointer() < rptr.GetRawPointer();
	}

	template <typename X, typename Y>
	bool operator< (const ptr< X >& lptr, Y* raw)
	{
		return lptr.GetRawPointer() < raw;
	}

	template <typename X, typename Y>
	bool operator< (X* raw, const ptr< Y >& rptr)
	{
		return raw < rptr.GetRawPointer();
	}

	
}
