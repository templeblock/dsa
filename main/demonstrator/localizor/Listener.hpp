/*
 * Listener.hpp
 *
 *  Created on: 03.03.2013
 *      Author: Bigontina
 */

#ifndef LISTENER_HPP_
#define LISTENER_HPP_

/*
 * This class is used to listen to events of the LocalizorSys class.
 * Inherit from this class and add it as a listener to LocalizorSys.
 */
class Listener {

public:

	enum Event
	{
	    ITERATION = 0
	};

	virtual void eventHandler(Listener::Event) = 0;

};


#endif /* LISTENER_HPP_ */
