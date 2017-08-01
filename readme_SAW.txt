									Situational Awarness Wearable (SAW)

SAW is the base class for a simple mid level manager for an Object Detection System (ODS). Provides a format for collecting the defining properties of an ODS, generating the required control systems, 
and managing their high level activity. The primary operational aspects of this system are the check and reset methods, which toggle a single operational cycle of the device and force it back to its default condition 
respectivly. The reset method in particular has a critical role;  it is intended for use as an interrupt service routine paired with the user control pin. This allows the device to be deactivated at any point in its operation, 
without missing any user prompts. The check function defines the behavior for a single operational cycle, from input to physical response. The results of each check are persistant across calls until reset occurs, from 
the top level controller's perspective, running the ODS simply requires calling this function repeatedly and allowing the internal components to determine the appropriate behavior given the system's current state.



FORMAT:
	       SAW serves two primary purposes: to assemble basic ODS components into custom configurations, and to provide a basic control system to run them. The first can be accomplished by SAW derivatives by either passing
the data through the constructor and using the default initializer init, or by modifying it post construction using the customInit function. The control system is split between two functions, check and reset. All low level managment should be 
handled within these methods where possible, with  more complicated behaviors being defined based on the how and when check and reset are called.  
	check serves as the defintion for  a single operational cycle for a given configuration. By default all related data is held constant between calls, allowing basic continuous operation to occur by running check until the the end 
conditions for the active phase are met. reset handles how the system should behave when entering or exiting the active phase and should be treated as an Interrupt Service Routine. Most interrupt handlers do not allow class functions
to be used as ISRs directly, so reset should be wrapped within a top level method bound to the user control pin. 




WORKFLOW:
	        After the constructor for the SAW derivative class is called, and the appropriate initializer used to generate the ODS components, the check function should be wrapped in any additional control conditions to be call
continuously for as long as the device is operational. When the user control pin provides the required signal, reset will assume control long enough to set the ODS components to their default conditions and shift the operational phase 
between active and inactive (based on the state it was in when called). Additional bahviors or conditions may be placed within the wrapper around reset to modify the overall behavior of the system. 


ADDENDUM:
	        - To avoid timing issues while using the reset method, wrap it in an external interrupt service routine (packaged with any additonal system level behavior required)  and pair to the input pin.
	        - Keep the behavior of the check and reset methods as basic as possible to allow its use in multiple contexts. Where specific cases require more complex behavior, attempt to attempt to 
		implement it as conditions on when check can occur at the top level before baking it in to its basic behavior. 
	        - customInit requires a complete definition for the ODS in question, including values that only have one possible value for a specific system. It is advised that those variables be given a 
		default value and the associated parameter in the customInit function be ignored to avoid invalid formats.
	        - Use a edge based interrupt handler for the user control pin. 
	        - Active phase refers to the period in which the system is reading and responding to the environment and has not yet met any end conditons
			- By default, the end conditions are a call to reset (indicating a shutdown command) and detecting an object that is closer 
				than the minimum distance defined by the proximity threshold. The first results in moving to the inactive state, while the 
				second freezes the system at the current settings until reset is called.  
	       - If using multiple SAW type ODS controllers, multiple reset  functions can be placed within the same wrapper ISR to synchronize behavior
		