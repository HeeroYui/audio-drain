/** @file
 * @author Edouard DUPIN 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#ifndef __DRAIN_LMS_H__
#define __DRAIN_LMS_H__

#include <etk/types.h>
#include <etk/chrono.h>

namespace drain {
	/**
	 * @brief Least Mean Square (LMS) algorithm "echo canceller"
	 * base on publication: http://www.arpapress.com/Volumes/Vol7Issue1/IJRRAS_7_1_05.pdf
	    Shcématic description:
	                                            /                                                                 
	                                     o---o /|                                                                 
	                      _feedback      |   |/ |                                                                 
	                   >---------------->|   |  | >~~~~~~~~o                                                      
	                         x(n)        |   |\ |          |                                                      
	                                     o---o \|          |                                                      
	                                            \      o--------0                                                 
	                                                   |        |    Environement                                 
	                                                   |   u(n) |  transfert fonction                             
	                                                   |        |                                                 
	                                                   o--------o                                                 
	                                                       |                                                      
	                                                       |                                                      
	                                     o---o ___         |                                                      
	                      _microphone    |   |/   \ <~~~~~~o                                                      
	                   <----------------<|   |    | <~~~~~~~~~~~~< Noise                                          
	                         d(n)        |   |\___/ <~~~~~~0                                                      
	                                     o---o             |                                                      
	                                                       |                                                      
	                                                       o~~~~~< Usefull signal                                 
	                                                                    s(n)                                      
	                                                                                                              
	    LMS Algorithm:
	                                                                                                              
	       _microphone -----------------------------o                                                             
	           d(n)                                 |                                                             
	                                                |                                                             
	                           o--------o           |    o-------------o                                          
	 o---> filter    --------->|        |           o--->|             |                                          
	 |        û(n)             | convol-|                | d(n) - y(n) |----> e(n) -------> echo-cancelled output 
	 |                         | -ution |----> y(n) ---->|             |        |                                 
	 |     _feedback -----o--->|        |                o-------------o        |                                 
	 |        x(n)        |    o--------o                                       |                                 
	 |                    |                                                     |                                 
	 |                    |         o----------------------------------o        |                                 
	 |                    |         |                                  |        |                                 
	 |                    o-------->|                                  |<-------o                                 
	 |                              |  û(n+1) =   û(n)                 |                                          
	 |                              |           + 2 * mu * e(n) * x(n) |                                          
	 |                              |                                  |                                          
	 |                              o----------------------------------o                                          
	 |                                            |                                                               
	 |                                            |                                                               
	 o--------------------------------------------o                                                               
	                                                                                                              
	                                                                                                              
	                                                                                                              
	*/
	class Lms {
		public:
			/**
			 * @brief Constructor
			 */
			Lms(void);
			/**
			 * @brief Destructor
			 */
			~Lms(void);
		public:
			/**
			 * @brief Reset filter history and filter
			 */
			void reset(void);
			/**
			 * @brief Process 16 bit LMS (input 16 bits)
			 * @param[in,out] _output output data of the LMS
			 * @param[in] _feedback Input feedback of the signal: x(n)
			 * @param[in] _microphone Input Microphone data: d(n)
			 */
			bool process(int16_t* _output, const int16_t* _feedback, const int16_t* _microphone, int32_t _nbSample);
			/**
			 * @brief Process float LMS
			 * @param[in,out] _output output data of the LMS
			 * @param[in] _feedback Input feedback of the signal: x(n)
			 * @param[in] _microphone Input Microphone data: d(n)
			 */
			bool process(float* _output, const float* _feedback, const float* _microphone, int32_t _nbSample);
		protected:
			/**
			 * @brief Process a single value of the LMS
			 * @param[in] _feedback Pointer on the feedback data (with history and at the n(th) position
			 * @param[in] _microphone Microphone single sample [-1..1]
			 * @return New output value [-1..1]
			 */
			float processValue(float* _feedback, float _microphone);
		public:
			/**
			 * @brief Set filter size with specifing the filter temporal size and his samplerate
			 * @param[in] _sampleRate Current sample rate to apply filter
			 * @param[in] _time Time of the filter size
			 */
			void setFilterSize(size_t _sampleRate, std11::chrono::microseconds _time);
			/**
			 * @brief Set filter size in number of sample
			 * @param[in] _nbSample Sample size of the filter
			 */
			void setFilterSize(size_t _nbSample);
			/**
			 * @brief Set Mu value for basic LMS value
			 * @param[in] _val new mu value
			 */
			void setMu(float _val);
		private:
			std::vector<float> m_filter; //!< Current filter
			std::vector<float> m_feedBack; //!< Feedback history
			float m_mu; //!< mu step size
		public:
			// for debug only:
			std::vector<float> getFilter() {
				return m_filter;
			}
	};
}

#endif
