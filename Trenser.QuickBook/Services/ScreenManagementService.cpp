#include <vector>
#include <iomanip>
#include <sstream>
#include "ScreenManagementService.h"
#include "Factory.h"

/*
* Function Name : ScreenManagementService
* Description   : Initializes the screen management service and connects it with the datastore.
* Parameters    : None
* Return Type   : Constructor
*/
ScreenManagementService::ScreenManagementService()
	: m_dataStore(DataStore::getInstance())
{ }