# SC2 simulation



## TODOs:
* **Register team and assign races to each developer**
* **UML digrams**
* 3 virtual classes: Resource, Building, Units
* Races and corresponding tech tree handling
* Class/functions executing simulation steps
* CSV reader for tech trees
* JSON output
* Fixed point class
* actually Building and Unit classes can be instantiated, not virtual. They have a member called buildingType/unitType which contains a string from the .csv file (e.g. "command_center" or "marine")
* minerals, gas, supply should be managed in one class where they are incremented each timestep, or reduced when building something
* The csv reader can parse the data to a static array of structs
