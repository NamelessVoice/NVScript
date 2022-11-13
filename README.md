# NVScript
NVScript is a Dark Engine script module for Thief 2, Thief 1, and System Shock 2.

# About the scripts

## Script parameters
These scripts make use of the **Editor->Design Note** (Thief) / **Script->ObjList Args** (System Shock 2) property for their configuration.  
The property can be used to hold any number of parameters, specified as name-value pairs, for the scripts to use - first the parameter name, then an equals sign, then the desired value (in quotes if it is a string).  
Multiple parameters can be defined by separating them with a semicolon.    

#### Example:

    NVRelayTrapOn="ExampleMessage"; NVRelayTrapOff="OtherMessage"; NVRelayTrapCount=3
    
When a script looks for an integer or flag parameter, you can have it read a quest variable and use its value in place of the parameter. After the equals sign, type a dollar sign ($) and the name of the quest variable (there may not be space between the dollar sign and the variable name.)  
For example, if you wanted to have an NVRelayTrap which fired a number of times based on the number entered into a combination lock/odometer combo using the **combo** quest variable, you could specify `NVRelayTrapRepeat=$combo`  
Note that if you want to supply the name of quest variable as a script parameter, rather than use the current value of the quest variable, then the dollar sign should not be used.  

For any parameter that specifies the name or number of an object for the script to affect, you can use `[me]` to use the object with the script, or `[source]` to use the object that sent the triggering message (the latter may not work in all circumstances). You can also precede the name of an archetype or metaproperty with the `^` symbol to use the nearest object that inherits from the one specified. For example, `"^Marker"` will affect the nearest object that descends from the *Marker* archetype, while `"^FrobInert"` will affect the nearest object that has the *FrobInert* metaproperty (either itself or on one of its archetypes).

In this document, script parameters which are **bolded** must be specified for the script to function: `NormalParameter`; **`RequiredParameter`**

## NVTrap scripts
All the scripts labelled as **NVTrap**s (those that respond to **TurnOn** / **TurnOff** messages) inherit behaviours from a generic 'NVTrap' base script, which allows you to use parameters to imitate an independent **Script->Trap Control Flags** property for each script, even when you have several on the same object. Each 'Trap' script understands five parameters, (Where `[ScriptName]` is the name of the script as you entered it into the **S->Scripts** property.):

`[ScriptName]On`  
Use this parameter to specify which script message should activate the trap. The default is TurnOn.

`[ScriptName]Off`  
Use this parameter to specify which script message should deactivate the trap. The default is TurnOff.

`[ScriptName]Count`  
Use this parameter to specify the maximum number of times that the script will work. You can use [ScriptName]Count=1 to emulate the *Once* **Trap Control Flag**. The default is 0 (infinite).  
Both **TurnOn** and **TurnOff** are counted by default. You can use `[ScriptName]CountOnly=1` to ignore **TurnOff**, and `[ScriptName]CountOnly=2` to ignore **TurnOn**. Sending the trap a **ResetCount** message will reset the counter.

You can use `[ScriptName]On="TurnOff";[ScriptName]Off="TurnOn"` to simulate the *Invert* **Trap Control Flag**; `[ScriptName]On="Null"` can be used to simulate *NoOn* and `[ScriptName]Off="Null"` can be used to simulate *NoOff*  
Remember that if you specify `[ScriptName]On="TurnOff"` without specifying `[ScriptName]Off`, then **TurnOff** will be specified for both values.

`[ScriptName][On/Off]Capacitor`  
Use this parameter to specify the number of times that a trap will need to receive its triggering message before it activates. For example, an NVRelayTrap with `NVRelayTrapCapacitor=3` will only relay every third message, while one with `NVRelayTrapOnCapacitor=4; NVRelayTrapOffCapacitor=2` will relay every second *TurnOff* message but only every fourth *TurnOn* message. The default is 1.  
`[ScriptName][On/Off]CapacitorFalloff` can be used to specify the time, in milliseconds, that it takes for the trap to "lose charge", and the activation count to go back down by one activation.

The scripts that this applies to are denoted in this document by the words NVTrap in parentheses after their names.  
Please note that the standard **Trap Control Flags** property is not used or supported.

## NVTrigger scripts
All the scripts labelled as NVTriggers (those that respond to events by sending **TurnOn** and **TurnOff** messages) inherit behaviours from a generic 'NVTrigger' base script, which allows you to use parameters to imitate an independent **Script->Trap Control Flags** property for each script, even when you have several on the same object. Each 'Trigger' script understands five parameters, (Where `[ScriptName]` is the name of the script as you entered it into the S->Scripts property.):

`[ScriptName]TOn`  
Use this parameter to specify which script message the trigger should send when activated. The default is **TurnOn**.

`[ScriptName]TOff`  
Use this parameter to specify which script message the trigger should send when deactivated. The default is **TurnOff**.

You can use trigger on and off parameters to specify a stim to send instead. To do this, first enter the intensity surrounded by square brackets, followed by the stim name. For example: `[ScriptName]TOn="[5.00]WaterStim"`.  
You can also specify a range of stims by specifying the minimum and maximum intensities, separated by a pipe. For example: `[ScriptName]TOn="[5.00|10.00]WaterStim"` will send a WaterStim with a random intensity between 5.00 and 10.00. Note that the result is a floating point number anywhere inside the range, it is *not* limited to whole numbers.

`[ScriptName]TCount`  
Use this parameter to specify the maximum number of times that the script will work. You can use `[ScriptName]TCount=1` to emulate the *Once* **Trap Control Flag**. The default is 0 (infinite).
Sending of both **TurnOn** and **TurnOff** messages are counted by default. You can use `[ScriptName]TCountOnly=1` to ignore *TurnOff*, and `[ScriptName]TCountOnly=2` to ignore *TurnOn*. Sending the trigger a **ResetTriggerCount** message will reset the counter.

`[ScriptName]FailChance`  
Use this parameter to specify a % chance that the trigger will fail (do nothing) when it is activated. For example: `[ScriptName]FailChance=35` will give the trigger a 35% chance to fail when activated.

`[ScriptName]TDest`  
Use this parameter to specify the object that the trigger should send messages to. Either specify a single object or archetype (see below), or use the name of a link type to broadcast the message along, preceded by an `&` symbol. The default is `[ScriptName]TDest="&ControlDevice"` (Thief) / `[ScriptName]TDest="&SwitchLink"` (System Shock 2). To have the message sent along a single random link of the specified type (rather than all links of that type), put a question mark (`?`) between the & and the link type.  
Example: `[ScriptName]TDest="&?ControlDevice"` will send the message down a random ControlDevice link.  
If you want the randomly chosen link to be deleted, then use `[ScriptName]KillLinks=1`.  
You can also use weighted random links via ``[ScriptName]TDest="&Weighted"`. This will examine **ScriptParams** links and treat their data (must be an integer) as the chance of that link being chosen. (A link with data of 2 will be chosen twice as often as one with data of 1). Weighted random only work with **ScriptParams** links.  

Finally, you can use either `*` or `@` to affect all objects that inherit from a specific archetype: `*` will only affect objects which are direct descendants, and `@` will affect all descendants.
For example: `[ScriptName]TDest="*Chest"` would affect all *Chest (-2571)* objects in the mission, but will not affect objects based on its descendant archetypes of *Safe (-5986)*, *VicHopeChest (-5773)*, *LC_Chest (-4067)*, and *SeaChest (-2817)*. `[ScriptName]TDest="@Chest"` would affect all of these objects.  
In addition, you can affect all objects within a specific radius. First enter a less-than symbol (`<`), then the radius, followed by a colon (`:`) and then the archetype name. For example: `[ScriptName]TDest="<5.00:Chest"` will affect all *Chest* objects or descendants withing a radius of 5.00 DromEd units. Note that the radius function will affect objects in the same way as `@` - in other words, all descendant archetypes will also be affected. By default, the radius is only calculated on the X and Y axis. To measure distance in 3-dimensional space, use the `{` symbol instead of the less-than symbol.

Below is a table with examples of each way that objects can be targeted:

| Parameter value | Object(s) to target                                                                                                                                                                                                                       | Applies to                          |
| --------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------- |
| ObjectName      | A specific object named *ObjectName*.                                                                                                                                                                                                     | Any parameter specifying an object. |
| 245             | A specific object with ID *245*.                                                                                                                                                                                                          | Any parameter specifying an object. |
| [me]            | This object where the script is running.                                                                                                                                                                                                  | Any parameter specifying an object. |
| [source]        | The source of the message that triggered this script.                                                                                                                                                                                     | Any parameter specifying an object. |
| ^Marker         | The closest object (relative to the object running the script) that descends from the Marker archetype.                                                                                                                                   | Any parameter specifying an object. |
| ^<20:Marker     | The closest object (relative to the object running the script) that descends from the Marker archetype and is within 20 units (XY axis only).                                                                                             | Any parameter specifying an object. |
|^{20:Marker      | The closest object (relative to the object running the script) that descends from the Marker archetype and is within 20 units (3-dimensional space).                                                                                      | Any parameter specifying an object. |
| &ControlDevice  | All objects that this object has ControlDevice links to.                                                                                                                                                                                  | An NVTrigger script's TDest         |
| &?ControlDevice | One random object that this object has a ControlDevice link to. The link to that object will be removed afterwards if `[ScriptName]KillLinks=1`                                                                                           | An NVTrigger script's TDest         |
| &Weighted       | One random object that this object has a ScriptParams link to. The link is using a weighted random using the value of the ScriptParams link, with a link with a value of 2 being twice as likely to be chosen as one with a value of 1.   | An NVTrigger script's TDest         |
| *Marker         | Every object in the mission that is based on the Marker archetype.                                                                                                                                                                        | An NVTrigger script's TDest         |
| @Marker         | Every object in the mission that is based on the Marker archetype or one of its descendants.                                                                                                                                              | An NVTrigger script's TDest         |
| <20:Marker      | Every object within 20 units (horizontal distance only) that is based on the Marker archetype or one of its descendants.                                                                                                                  | An NVTrigger script's TDest         |
| {20:Marker      | Every object within 20 units (in all three dimensions) that is based on the Marker archetype or one of its descendants.                                                                                                                   | An NVTrigger script's TDest         |

## Targetting scripts
Some scripts allow you to specify the object(s) that they should affect. You can do this via the `[ScriptName]Target` parameter, which generally defaults to `[me]` in most cases. For example, to have NVMetaTrap add and remove its metaproperty on objects linked from its object via ControlDevice links, you would use `NVMetaTrapTarget="&ControlDevice"`. Scripts which can be targetted in this way are marked as Targetable in their description. 


# Scripts
For a detailed description of each script and its parameters, see the NVScript.html which is part of each release zip.
