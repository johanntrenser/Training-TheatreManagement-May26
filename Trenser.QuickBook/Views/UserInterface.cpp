/*
 * File: UserInterface.cpp
 * Description: Implements the UserInterface class responsible for handling
 *              user interactions with the Quick Book - Theatre Management System.
 * Author: Trenser
 * Created: 20 May 2026
 */
#include <iostream>
#include <stdexcept>
#include <string>
#include<iomanip>
#include <ctime>
#include <sstream>
using namespace std;
#include "InputHelper.h"
#include "OutputHelper.h"
#include "UserInterface.h"
#include "Validator.h"
#include "Enums.h"
#include "TimeStamp.h"

/*
 * Function: UserInterface::UserInterface
 * Description: Default constructor. Initializes the UserInterface by creating
 *              a new Controller instance and wiring it with all required
 *              management services including authentication, user, theatre,
 *              movie, show, booking, payment, ticket, notification, log,
 *              screen, seat, and refund management.
 * Parameters: None
 * Returns: None
 */
UserInterface::UserInterface()
{
	m_controller = new Controller(new AuthenticationManagementService(),
		new UserManagementService(),
		new TheatreManagementService(),
		new MovieManagementService(),
		new ShowManagementService(),
		new BookingManagementService(),
		new PaymentManagementService(),
		new TicketManagementService(),
		new NotificationManagementService(),
		new LogManagementService(),
		new ScreenManagementService(),
		new SeatManagementService(),
		new RefundManagementService());
}

/*
 * Function: run
 * Description: Starts the main user interface loop. Displays the menu and
 *              processes user input until the user chooses to exit.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::run()
{
	bool isMenuActive = true;
	while (isMenuActive)
	{
		try
		{
			int choice;
			util::clear();
			util::readValueWithRetry(choice, "Quick Book - Theatre Management System\n1. Login\n2. Register\n3. Exit\nEnter Your Choice: ");
			if (!handleOperation(choice))
			{
				isMenuActive = false;
			}
		}
		catch (const runtime_error& e)
		{
			cout << "Runtime Exception: " << e.what();
		}
		catch (const exception& e)
		{
			cout << "Exception : " << e.what() << endl;
		}
	}
}

/*
 * Function: handleOperation
 * Description: Handles the operation selected by the user from the main menu.
 * Parameters:
 *    choice - The integer value representing the user's menu selection
 * Returns:
 *    bool - true if the menu should remain active,
 *           false if the user chooses to exit
 */
bool UserInterface::handleOperation(unsigned short choice)
{
	switch (choice)
	{
		case 1:
			login();
			break;
		case 2:
			registerUser();
			break;
		case 3:
			cout << "Exiting..." << endl;
			return false;
		default:
			cout << "Enter a valid choice!" << endl;
			util::pressEnterToContinue();
	}
	return true;
}

/*
 * Function: login
 * Description: Prompts the user for email and password, attempts authentication
 *              via the Controller, and directs the user to the appropriate menu
 *              based on their role (Admin, Theatre Owner, Customer).
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::login()
{
	string email, password;
	util::clear();
	util::readValueWithRetry(email, "Enter email: ");
	util::isEmailValid(email);
	util::readValueWithRetry(password, "Enter password: ");
	const pair<Enums::LoginStatus, Enums::UserType> authenticationContext = m_controller->login(email, password);
	const Enums::LoginStatus loginStatus = get<0>(authenticationContext);
	const Enums::UserType userType = get<1>(authenticationContext);
	if (loginStatus == Enums::LoginStatus::USER_NOT_FOUND)
	{
		cout << "Error: Invalid Credentials! Try Again\n";
		util::pressEnterToContinue();
		return;
	}
	if (loginStatus == Enums::LoginStatus::INVALID_PASSWORD)
	{
		cout << "Error: Invalid Credentials! Try Again\n";
		util::pressEnterToContinue();
		return;
	}
	if (loginStatus == Enums::LoginStatus::USER_ALREADY_LOGGED_IN)
	{
		cout << "Error: User is already logged in! Try again\n";
		util::pressEnterToContinue();
		return;
	}
	util::clear();
	switch (userType)
	{
		case Enums::UserType::ADMIN:
		{
			handleAdminMenuOperation();
			break;
		}
		case Enums::UserType::THEATRE_OWNER:
		{
			handleTheatreOwnerMenuOperation();
			break;
		}
		case Enums::UserType::CUSTOMER:
		{
			handleCustomerMenuOperation();
			break;
		}
	}
	if (m_controller->getAuthenticatedUser() != nullptr)
	{
		m_controller->logout();
	}
};

/*
 * Function: registerUser
 * Description: Prompts the user to select a user type, collects user details,
 *              and registers the user through the Controller.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::registerUser()
{
	string userName, email, password, phoneNumber;
	Enums::UserType userType = Enums::UserType::CUSTOMER;
	unsigned short choice;
	userTypesMenu();
	util::readValueWithRetry(choice, "Enter a choice: ");
	switch (choice)
	{
		case 1:
			userType = Enums::UserType::CUSTOMER;
			break;
		case 2:
			userType = Enums::UserType::THEATRE_OWNER;
			break;
		default:
			cout << "Invalid Choice. Please Try again!" << endl;
			util::pressEnterToContinue();
			util::clear();
			return;
	}
	handleUserDetailsInput(userName, email, password, phoneNumber);
	Enums::ProcessStatus status = m_controller->registerUser(userName, email, password, phoneNumber, userType);
	if (status == Enums::ProcessStatus::SUCCESS)
	{
		cout << "User registered successfully!" << endl;
		util::pressEnterToContinue();
		util::clear();
	}
	else if (status == Enums::ProcessStatus::EMAIL_ALREADY_EXISTS)
	{
		cout << "Email Already exists! Registration failed." << endl;
		util::pressEnterToContinue();
		util::clear();
	}
	else if (status == Enums::ProcessStatus::PHONE_NUMBER_ALREADY_EXISTS)
	{
		cout << "Phone Number Already exists! Registration failed." << endl;
		util::pressEnterToContinue();
		util::clear();
	}
	else
	{
		cout << "User could not be registered!" << endl;
		util::pressEnterToContinue();
		util::clear();
	}
}

/*
 * Function: userTypesMenu
 * Description: Displays the available user types for registration and prompts
 *              the user to make a selection.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::userTypesMenu()
{
	cout << "-------------------User Type List-------------------" << endl;
	cout << "1. Customer" << endl;
	cout << "2. Theatre Owner" << endl;
}

/*
Function Name : adminMenu
Description   : Displays the main menu options available to Admin users.
Parameters    : None
Return Type   : void
*/
void UserInterface::adminMenu()
{
	cout << "Admin Menu" << endl;
	cout << "------------------------" << endl;
	cout << " 1.  User Management" << endl;
	cout << " 2.  Movie Management" << endl;
	cout << " 3.  Theatre Management" << endl;
	cout << " 4.  Show Management" << endl;
	cout << " 5.  Ticket Management" << endl;
	cout << " 6.  View Logs" << endl;
	cout << " 7.  View Profile" << endl;
	cout << " 8.  Change Password" << endl;
	cout << " 9.  View Notifications" << endl;
	cout << " 10. Logout" << endl;
	cout << "------------------------" << endl;
}

/*
Function Name : handleAdminMenuOperation
Description   : Handles Admin menu operations in a loop until logout.
				Routes to submenus for user, movie, theatre, show, and ticket management.
Parameters    : None
Return Type   : void
*/
void UserInterface::handleAdminMenuOperation()
{
	bool isMenuActive = true;
	unsigned short choice;
	while (isMenuActive)
	{
		if (m_controller->checkAndHandleForcedLogout())
		{
			showForceLogoutMessage(isMenuActive);
			return;
		}
		util::clear();
		adminMenu();
		util::readValueWithRetry(choice, "Enter an option: ");
		switch (choice)
		{
			case 1:
			{
				adminUserManagementMenu();
				break;
			}
			case 2:
			{
				adminMovieManagementMenu();
				break;
			}
			case 3:
			{
				adminTheatreManagementMenu();
				break;
			}
			case 4:
			{
				adminShowManagementMenu();
				break;
			}
			case 5:
			{
				adminTicketManagementMenu();
				break;
			}
			case 6:
			{
				viewLogsByType();
				break;
			}
			case 7:
			{
				viewProfile();
				break;
			}
			case 8:
			{
				changePassword();
				break;
			}
			case 9:
			{
				viewNotifications();
				break;
			}
			case 10:
			{
				isMenuActive = false;
				break;
			}
			default:
			{
				cout << "Invalid choice. Please try again!" << endl;
				util::pressEnterToContinue();
				break;
			}
		}
	}
}

/*
Function Name : adminUserManagementMenu
Description   : Displays the Admin User Management submenu and handles operations
				interactively in a loop until the user chooses to go back.
				Provides options to create, view, update, deactivate, reactivate,
				and check user status, as well as view notifications.
Parameters    : None
Return Type   : void
*/
void UserInterface::adminUserManagementMenu()
{
	bool isMenuActive = true;
	unsigned short choice;
	while (isMenuActive)
	{
		util::clear();
		cout << "User Management" << endl;
		cout << "------------------------" << endl;
		cout << "1. Create User" << endl;
		cout << "2. View All Users" << endl;
		cout << "3. Update User Details" << endl;
		cout << "4. Deactivate User" << endl;
		cout << "5. Reactivate User" << endl;
		cout << "0. Back" << endl;
		util::readValueWithRetry(choice, "Enter an option: ");
		switch (choice)
		{
			case 1:
			{
				createUser();
				break;
			}
			case 2:
			{
				viewAllUsers();
				break;
			}
			case 3:
			{
				updateUserDetails();
				break;
			}
			case 4:
			{
				deactivateUser();
				break;
			}
			case 5:
			{
				reactivateUser();
				break;
			}
			case 0:
			{
				isMenuActive = false;
				break;
			}
			default:
			{
				cout << "Invalid choice!" << endl;
				util::pressEnterToContinue();
				break;
			}
		}
	}
}

/*
* Function Name : adminMovieManagementMenu
* Description   : Displays the Admin Movie Management submenu and handles operations
*                 interactively in a loop until the user chooses to go back.
*                 Provides options to add, update, list, search, deactivate, and
*                 reactivate movies. Uses input retry logic to ensure valid menu
*                 selection.
* Parameters    : None
* Return Type   : void
*/
void UserInterface::adminMovieManagementMenu()
{
	bool isMenuActive = true;
	unsigned short choice;
	while (isMenuActive)
	{
		util::clear();
		cout << "Movie Management" << endl;
		cout << "------------------------" << endl;
		cout << "1. Add Movie" << endl;
		cout << "2. Update Movie" << endl;
		cout << "3. List All Movies" << endl;
		cout << "4. Search Movie" << endl;
		cout << "5. Deactivate Movie" << endl;
		cout << "6. Reactivate Movie" << endl;
		cout << "0. Back" << endl;
		util::readValueWithRetry(choice, "Enter an option: ");
		switch (choice)
		{
			case 1:
			{
				addMovie();
				break;
			}
			case 2:
			{
				updateMovie();
				break;
			}
			case 3:
			{
				displayAllMovies();
				break;
			}
			case 4:
			{
				searchMovie();
				break;
			}
			case 5:
			{
				deactivateMovie();
				break;
			}
			case 6:
			{
				activateMovie();
				break;
			}
			case 0:
			{
				isMenuActive = false;
				break;
			}
			default:
			{
				cout << "Invalid choice!" << endl;
				util::pressEnterToContinue();
				break;
			}
		}
	}
}

/*
Function Name : adminMovieManagementMenu
Description   : Displays the Admin Movie Management submenu and handles operations
				interactively in a loop until the user chooses to go back.
				Provides options to add, update, list, search, deactivate, and
				reactivate movies.
Parameters    : None
Return Type   : void
*/
void UserInterface::adminTheatreManagementMenu()
{
	bool isMenuActive = true;
	unsigned short choice;
	while (isMenuActive)
	{
		util::clear();
		cout << "Theatre Management" << endl;
		cout << "------------------------" << endl;
		cout << "1. Validate Theatre Request" << endl;
		cout << "2. Deactivate Theatre" << endl;
		cout << "3. Reactivate Theatre" << endl;
		cout << "4. Search Theatre" << endl;
		cout << "5. List All Theatres" << endl;
		cout << "0. Back" << endl;
		util::readValueWithRetry(choice, "Enter an option: ");
		switch (choice)
		{
			case 1:
			{
				validateTheatreRequest();
				break;
			}
			case 2:
			{
				deactivateTheatreByAdmin();
				break;
			}
			case 3:
			{
				reactivateTheatreByAdmin();
				break;
			}
			case 4:
			{
				searchTheatre();
				break;
			}
			case 5:
			{
				listAllTheatres();
				break;
			}
			case 0:
			{
				isMenuActive = false;
				break;
			}
			default:
			{
				cout << "Invalid choice!" << endl;
				util::pressEnterToContinue();
				break;
			}
		}
	}
}

/*
Function Name : adminShowManagementMenu
Description   : Displays the Admin Show Management submenu and handles operations
				interactively in a loop until the user chooses to go back.
				Provides options to view all shows, check show status, and
				list shows for a specific movie.
Parameters    : None
Return Type   : void
*/
void UserInterface::adminShowManagementMenu()
{
	bool isMenuActive = true;
	unsigned short choice;
	while (isMenuActive)
	{
		util::clear();
		cout << "Show Management" << endl;
		cout << "------------------------" << endl;
		cout << "1. List Shows For a Movie" << endl;
		cout << "0. Back" << endl;
		util::readValueWithRetry(choice, "Enter an option: ");
		switch (choice)
		{
			case 1:
			{
				listShowsForAMovie();
				break;
			}
			case 0:
			{
				isMenuActive = false;
				break;
			}
			default:
			{
				cout << "Invalid choice!" << endl;
				util::pressEnterToContinue();
				break;
			}
		}
	}
}

/*
Function Name : adminTicketManagementMenu
Description   : Displays the Admin Ticket Management submenu and handles operations
				interactively in a loop until the user chooses to go back.
				Provides options to view all tickets and check ticket status.
Parameters    : None
Return Type   : void
*/
void UserInterface::adminTicketManagementMenu()
{
	bool isMenuActive = true;
	unsigned short choice;
	while (isMenuActive)
	{
		util::clear();
		cout << "Ticket Management" << endl;
		cout << "------------------------" << endl;
		cout << "1. View All Tickets" << endl;
		cout << "0. Back" << endl;
		util::readValueWithRetry(choice, "Enter an option: ");
		switch (choice)
		{
			case 1:
			{
				viewAllTickets();
				break;
			}
			case 0:
			{
				isMenuActive = false;
				break;
			}
			default:
			{
				cout << "Invalid choice!" << endl;
				util::pressEnterToContinue();
				break;
			}
		}
	}
}

/*
Function Name : theatreOwnerMenu
Description   : Displays the main menu options available to Theatre Owner users.
				Provides access to theatre, show, and booking management, as well
				as notifications, profile, user details update, password change,
				and logout.
Parameters    : None
Return Type   : void
*/
void UserInterface::theatreOwnerMenu()
{
	cout << "Theatre Owner Menu" << endl;
	cout << "------------------------" << endl;
	cout << "1. Theatre Management" << endl;
	cout << "2. Show Management" << endl;
	cout << "3. Booking Management" << endl;
	cout << "4. View Notifications" << endl;
	cout << "5. View Profile" << endl;
	cout << "6. Update User Details" << endl;
	cout << "7. Change Password" << endl;
	cout << "8. Logout" << endl;
	cout << "------------------------" << endl;
}

/*
Function Name : handleTheatreOwnerMenuOperation
Description   : Handles Theatre Owner menu operations in a loop until logout.
				Routes to submenus for theatre, show, and booking management,
				as well as notifications, profile viewing, user detail updates,
				and password changes. Provides a logout option to exit the menu.
Parameters    : None
Return Type   : void
*/
void UserInterface::handleTheatreOwnerMenuOperation()
{
	bool isMenuActive = true;
	unsigned short choice;
	while (isMenuActive)
	{
		if (m_controller->checkAndHandleForcedLogout())
		{
			showForceLogoutMessage(isMenuActive);
			return;
		}
		util::clear();
		theatreOwnerMenu();
		util::readValueWithRetry(choice, "Enter an option: ");
		switch (choice)
		{
			case 1:
			{
				ownerTheatreManagementMenu();
				break;
			}
			case 2:
			{
				ownerShowManagementMenu();
				break;
			}
			case 3:
			{
				ownerBookingManagementMenu();
				break;
			}
			case 4:
			{
				viewNotifications();
				break;
			}
			case 5:
			{
				viewProfile();
				break;
			}
			case 6:
			{
				updateUserDetails();
				break;
			}
			case 7:
			{
				changePassword();
				break;
			}
			case 8:
			{
				isMenuActive = false;
				break;
			}
			default:
			{
				cout << "Invalid choice. Please try again!" << endl;
				util::pressEnterToContinue();
				break;
			}
		}
	}
}

/*
Function Name : ownerTheatreManagementMenu
Description   : Displays the Theatre Owner Theatre Management submenu and handles
				operations interactively in a loop until the user chooses to go back.
				Provides options to add, view, update, deactivate, reactivate, search,
				and list theatres, as well as manage movies within a theatre.
Parameters    : None
Return Type   : void
*/
void UserInterface::ownerTheatreManagementMenu()
{
	bool isMenuActive = true;
	unsigned short choice;
	while (isMenuActive)
	{
		util::clear();
		cout << "Theatre Management" << endl;
		cout << "------------------------" << endl;
		cout << "1. Add Theatre" << endl;
		cout << "2. View Theatre Details" << endl;
		cout << "3. Update Theatre Details" << endl;
		cout << "4. Deactivate Theatre" << endl;
		cout << "5. Reactivate Theatre" << endl;
		cout << "6. Search Theatre" << endl;
		cout << "7. List All Theatres" << endl;
		cout << "8. Add Movie to Theatre" << endl;
		cout << "9. View Movies in Theatre" << endl;
		cout << "10. Remove Movies in Theatre" << endl;
		cout << "11. Screen Management" << endl;
		cout << "0. Back" << endl;
		util::readValueWithRetry(choice, "Enter an option: ");
		switch (choice)
		{
			case 1:
			{
				addTheatre();
				break;
			}
			case 2:
			{
				viewTheatreDetails();
				break;
			}
			case 3:
			{
				updateTheatre();
				break;
			}
			case 4:
			{
				deactivateTheatreByOwner();
				break;
			}
			case 5:
			{
				reactivateTheatreByOwner();
				break;
			}
			case 6:
			{
				searchTheatre();
				break;
			}
			case 7:
			{
				listAllOwnerTheatres();
				break;
			}
			case 8:
			{
				addMovieToTheatre();
				break;
			}
			case 9:
			{
				displayMoviesInTheatre();
				break;
			}
			case 10:
			{
				removeMovieFromTheatre();
				break;
			}
			case 11:
			{
				ownerScreenManagementMenu();
				break;
			}
			case 0:
			{
				isMenuActive = false;
				break;
			}
			default:
			{
				cout << "Invalid choice!" << endl;
				util::pressEnterToContinue();
				break;
			}
		}
	}
}

/*
* Function Name : ownerScreenManagementMenu
* Description   : Displays the Theatre Owner Screen Management submenu and handles
*                 operations interactively in a loop until the user chooses to go back.
*                 Provides options to add, view, update, deactivate, and reactivate screens,
*                 as well as manage seats within a selected screen. Ensures that the
*                 theatre ID is validated before performing operations and prompts
*                 the user for necessary input values such as screen name, dimensions,
*                 and seat price.
* Parameters    : None
* Return Type   : void
*/
void UserInterface::ownerScreenManagementMenu()
{
	bool isMenuActive = true;
	unsigned short choice;
	while (isMenuActive)
	{
		util::clear();
		cout << "Screen Management" << endl;
		cout << "------------------------" << endl;
		cout << "1. Add Screen" << endl;
		cout << "2. View Screens" << endl;
		cout << "3. Update Screen Name" << endl;
		cout << "4. Deactivate Screen" << endl;
		cout << "5. Reactivate Screen" << endl;
		cout << "6. Seat Management" << endl;
		cout << "0. Back" << endl;
		util::readValueWithRetry(choice, "Enter an option: ");
		if (choice == 0)
		{
			isMenuActive = false;
			break;
		}
		string theatreId;
		const vector<const Theatre*> theatres = m_controller->getCurrentOwnerTheatres();
		if (theatres.empty())
		{
			cout << "No theatres found!" << endl;
			util::pressEnterToContinue();
			break;
		}
		displayTheatreDetails(theatres);
		util::readValueWithRetry(theatreId, "Enter Theatre ID: ");
		if (isValidTheatreID(theatreId, theatres) == Enums::ProcessStatus::FAILED)
		{
			cout << "Invalid Theatre ID!" << endl;
			util::pressEnterToContinue();
			continue;
		}

		switch (choice)
		{
			case 1:
			{
				string name;
				int rows, columns;
				double amount;
				util::readValueWithRetry(name, "Enter Screen Name: ");
				util::readValueWithRetry(rows, "Enter Number of Rows: ");
				util::readValueWithRetry(columns, "Enter Number of Columns: ");
				util::readValueWithRetry(amount, "Enter Seat Price: ");
				addScreen(theatreId, name, rows, columns, amount);
				util::pressEnterToContinue();
				break;
			}
			case 2:
			{
				viewTheatreScreens(theatreId);
				util::pressEnterToContinue();
				break;
			}
			case 3:
			{
				string screenId, name;
				viewTheatreScreens(theatreId);
				util::readValueWithRetry(screenId, "Enter Screen ID: ");
				util::readValueWithRetry(name, "Enter New Name: ");
				updateScreenName(theatreId, screenId, name);
				util::pressEnterToContinue();
				break;
			}
			case 4:
			{
				string screenId;
				viewTheatreScreens(theatreId);
				util::readValueWithRetry(screenId, "Enter Screen ID to deactivate: ");
				deactivateScreen(theatreId, screenId);
				util::pressEnterToContinue();
				break;
			}
			case 5:
			{
				string screenId;
				viewTheatreScreens(theatreId);
				util::readValueWithRetry(screenId, "Enter Screen ID to reactivate: ");
				reactivateScreen(theatreId, screenId);
				util::pressEnterToContinue();
				break;
			}
			case 6:
			{
				string screenId, selectedScreenId = "";
				viewTheatreScreens(theatreId);
				util::readValueWithRetry(screenId, "Enter Screen ID for seat management: ");
				const vector<const Screen*> screens = m_controller->getScreensFromTheatre(theatreId);
				for (vector<const Screen*>::const_iterator iterator = screens.begin(); iterator != screens.end(); ++iterator)
				{
					if ((*iterator)->getScreenId() == screenId)
					{
						selectedScreenId = screenId;
						break;
					}
				}
				if (selectedScreenId.empty())
				{
					cout << "Invalid Screen ID!" << endl;
					util::pressEnterToContinue();
					break;
				}
				ownerSeatManagementMenu(selectedScreenId);
				break;
			}
			default:
			{
				cout << "Invalid choice!" << endl;
				util::pressEnterToContinue();
				break;
			}
		}
	}
}

/*
* Function Name : ownerSeatManagementMenu
* Description   : Displays the Theatre Owner Seat Management submenu and handles operations
*                 interactively in a loop until the user chooses to go back.
*                 Provides options to view and update seat layout, deactivate or reactivate
*                 specific seats, and ensures user input is validated before performing actions.
* Parameters    :
*     - Screen* screen : Pointer to the screen object whose seats are being managed.
* Return Type   : void
*/
void UserInterface::ownerSeatManagementMenu(const string& selectedScreenId)
{
	bool isMenuActive = true;
	unsigned short choice;
	while (isMenuActive)
	{
		util::clear();
		cout << "Seat Management" << endl;
		cout << "------------------------" << endl;
		cout << "1. View Seat Layout" << endl;
		cout << "2. Update Seat Layout" << endl;
		cout << "3. Deactivate Seat" << endl;
		cout << "4. Reactivate Seat" << endl;
		cout << "0. Back" << endl;
		util::readValueWithRetry(choice, "Enter an option: ");
		switch (choice)
		{
			case 1:
			{
				viewSeatLayout(selectedScreenId);
				util::pressEnterToContinue();
				break;
			}
			case 2:
			{
				int rows = 0, columns = 0;
				double amount = 0.0;
				util::readValueWithRetry(amount, "Enter Seat Price: ");
				updateSeatLayout(selectedScreenId, rows, columns, amount);
				util::pressEnterToContinue();
				break;
			}
			case 3:
			{
				string seatId;
				viewSeatLayout(selectedScreenId);
				util::readValueWithRetry(seatId, "Enter Seat ID to deactivate: ");
				deactivateSeat(selectedScreenId, seatId);
				util::pressEnterToContinue();
				break;
			}
			case 4:
			{
				string seatId;
				viewSeatLayout(selectedScreenId);
				util::readValueWithRetry(seatId, "Enter Seat ID to reactivate: ");
				reactivateSeat(selectedScreenId, seatId);
				util::pressEnterToContinue();
				break;
			}
			case 0:
			{
				isMenuActive = false;
				break;
			}
			default:
			{
				cout << "Invalid choice!" << endl;
				util::pressEnterToContinue();
				break;
			}
		}
	}
}

/*
Function Name : ownerShowManagementMenu
Description   : Displays the Theatre Owner Show Management submenu and handles
				operations interactively in a loop until the user chooses to go back.
				Provides options to add, update, cancel, view, and list shows,
				as well as check show status and list shows for a specific movie.
Parameters    : None
Return Type   : void
*/
void UserInterface::ownerShowManagementMenu()
{
	bool isMenuActive = true;
	unsigned short choice;
	while (isMenuActive)
	{
		util::clear();
		cout << "Show Management" << endl;
		cout << "------------------------" << endl;
		cout << "1. Add Show" << endl;
		cout << "2. Update Show" << endl;
		cout << "3. Cancel Show" << endl;
		cout << "4. View All Active Shows" << endl;
		cout << "5. View All Shows" << endl;
		cout << "6. View Show Status" << endl;
		cout << "7. List Shows For a Movie" << endl;
		cout << "0. Back" << endl;
		util::readValueWithRetry(choice, "Enter an option: ");
		switch (choice)
		{
			case 1:
			{
				addShow();
				break;
			}
			case 2:
			{
				updateShow();
				break;
			}
			case 3:
			{
				cancelShow();
				break;
			}
			case 4:
				displayActiveShows();
				break;
			case 5:
			{
				displayAllShows();
				break;
			}
			case 6:
			{
				viewShowStatus();
				break;
			}
			case 7:
			{
				listShowsForAMovie();
				break;
			}
			case 0:
			{
				isMenuActive = false;
				break;
			}
			default:
			{
				cout << "Invalid choice!" << endl;
				util::pressEnterToContinue();
				break;
			}
		}
	}
}

/*
Function Name : ownerBookingManagementMenu
Description   : Displays the Theatre Owner Booking Management submenu and handles
				operations interactively in a loop until the user chooses to go back.
				Provides options to view all bookings and display booking details.
Parameters    : None
Return Type   : void
*/
void UserInterface::ownerBookingManagementMenu()
{
	bool isMenuActive = true;
	unsigned short choice;
	while (isMenuActive)
	{
		util::clear();
		cout << "Booking Management" << endl;
		cout << "------------------------" << endl;
		cout << "1. View All Bookings" << endl;
		cout << "2. View Booking Details" << endl;
		cout << "3. View All Payment" << endl;
		cout << "4. View All Refunds" << endl;
		cout << "0. Back" << endl;
		util::readValueWithRetry(choice, "Enter an option: ");
		switch (choice)
		{
			case 1:
			{
				viewAllBookings();
				break;
			}
			case 2:
			{
				displayBookingDetails();
				break;
			}
			case 3:
			{
				viewAllPayments();
				break;
			}
			case 4:
			{
				viewRefunds();
				break;
			}
			case 0:
			{
				isMenuActive = false;
				break;
			}
			default:
			{
				cout << "Invalid choice!" << endl;
				util::pressEnterToContinue();
				break;
			}
		}
	}
}

/*
Function Name : customerMenu
Description   : Displays the main menu options available to Customer users.
				Provides access to browsing and booking movies/shows, viewing
				bookings and tickets, notifications, profile, user details update,
				password change, and logout.
Parameters    : None
Return Type   : void
*/
void UserInterface::customerMenu()
{
	cout << "Customer Menu" << endl;
	cout << "------------------------" << endl;
	cout << "1. Browse & Book" << endl;
	cout << "2. My Bookings" << endl;
	cout << "3. My Tickets" << endl;
	cout << "4. View Notifications" << endl;
	cout << "5. View Profile" << endl;
	cout << "6. Update User Details" << endl;
	cout << "7. Change Password" << endl;
	cout << "8. Logout" << endl;
	cout << "------------------------" << endl;
}

/*
Function Name : handleCustomerMenuOperation
Description   : Handles Customer menu operations in a loop until logout.
				Routes to submenus for browsing and booking movies/shows,
				viewing bookings and tickets, notifications, profile viewing,
				user detail updates, and password changes. Provides a logout
				option to exit the menu gracefully.
Parameters    : None
Return Type   : void
*/
void UserInterface::handleCustomerMenuOperation()
{
	bool isMenuActive = true;
	unsigned short choice;
	while (isMenuActive)
	{
		if (m_controller->checkAndHandleForcedLogout())
		{
			showForceLogoutMessage(isMenuActive);
			return;
		}
		util::clear();
		customerMenu();
		util::readValueWithRetry(choice, "Enter an option: ");
		switch (choice)
		{
			case 1:
			{
				customerBrowseMenu();
				break;
			}
			case 2:
			{
				customerBookingMenu();
				break;
			}
			case 3:
			{
				customerTicketMenu();
				break;
			}
			case 4:
			{
				viewNotifications();
				break;
			}
			case 5:
			{
				viewProfile();
				break;
			}
			case 6:
			{
				updateUserDetails();
				break;
			}
			case 7:
			{
				changePassword();
				break;
			}
			case 8:
			{
				isMenuActive = false;
				break;
			}
			default:
			{
				cout << "Invalid choice. Please try again!" << endl;
				util::pressEnterToContinue();
				break;
			}
		}
	}
}

/*
Function Name : customerBrowseMenu
Description   : Displays the Customer Browse & Book submenu and handles operations
				interactively in a loop until the user chooses to go back.
				Provides options to search movies, list shows for a movie,
				search theatres, and create bookings.
Parameters    : None
Return Type   : void
*/
void UserInterface::customerBrowseMenu()
{
	bool isMenuActive = true;
	unsigned short choice;
	while (isMenuActive)
	{
		util::clear();
		cout << "Browse & Book" << endl;
		cout << "------------------------" << endl;
		cout << "1. Search Movie" << endl;
		cout << "2. List Shows For a Movie" << endl;
		cout << "3. Search Theatre" << endl;
		cout << "4. Create Booking" << endl;
		cout << "0. Back" << endl;
		util::readValueWithRetry(choice, "Enter an option: ");
		switch (choice)
		{
			case 1:
			{
				searchMovie();
				break;
			}
			case 2:
			{
				listShowsForAMovie();
				break;
			}
			case 3:
			{
				searchTheatre();
				break;
			}
			case 4:
			{
				createBooking();
				break;
			}
			case 0:
			{
				isMenuActive = false;
				break;
			}
			default:
			{
				cout << "Invalid choice!" << endl;
				util::pressEnterToContinue();
				break;
			}
		}
	}
}

/*
Function Name : customerBookingMenu
Description   : Displays the Customer Booking Management submenu and handles operations
				interactively in a loop until the user chooses to go back.
				Provides options to view all bookings, view booking details,
				cancel bookings, and check payment status.
Parameters    : None
Return Type   : void
*/
void UserInterface::customerBookingMenu()
{
	bool isMenuActive = true;
	unsigned short choice;
	while (isMenuActive)
	{
		util::clear();
		cout << "My Bookings" << endl;
		cout << "------------------------" << endl;
		cout << "1. View All Bookings" << endl;
		cout << "2. View Booking Details" << endl;
		cout << "3. Cancel Booking" << endl;
		cout << "4. View All Payment" << endl;
		cout << "5. View All Refunds" << endl;
		cout << "0. Back" << endl;
		util::readValueWithRetry(choice, "Enter an option: ");
		switch (choice)
		{
			case 1:
			{
				viewAllBookings();
				break;
			}
			case 2:
			{
				displayBookingDetails();
				break;
			}
			case 3:
			{
				cancelBooking();
				break;
			}
			case 4:
			{
				viewAllPayments();
				break;
			}
			case 5:
			{
				viewRefunds();
				break;
			}
			case 0:
			{
				isMenuActive = false;
				break;
			}
			default:
			{
				cout << "Invalid choice!" << endl;
				util::pressEnterToContinue();
				break;
			}
		}
	}
}

/*
Function Name : customerTicketMenu
Description   : Displays the Customer Ticket Management submenu and handles operations
				interactively in a loop until the user chooses to go back.
				Provides options to view active tickets, view ticket history,
				and check ticket status.
Parameters    : None
Return Type   : void
*/
void UserInterface::customerTicketMenu()
{
	bool isMenuActive = true;
	unsigned short choice;
	while (isMenuActive)
	{
		util::clear();
		cout << "My Tickets" << endl;
		cout << "------------------------" << endl;
		cout << "1. View Active Tickets" << endl;
		cout << "2. View Ticket History" << endl;
		cout << "0. Back" << endl;
		util::readValueWithRetry(choice, "Enter an option: ");
		switch (choice)
		{
			case 1:
			{
				viewActiveTicketDetails();
				break;
			}
			case 2:
			{
				viewTicketHistory();
				break;
			}
			case 0:
			{
				isMenuActive = false;
				break;
			}
			default:
			{
				cout << "Invalid choice!" << endl;
				util::pressEnterToContinue();
				break;
			}
		}
	}
}

/*
 * Function: getUniqueEmail
 * Description: Ensures that the provided email address is unique by validating
 *              against existing users in the system. Prompts the user to re‑enter
 *              the email until a unique and valid address is provided.
 * Parameters:
 *    email (std::string&) - Reference to the variable storing the user's email
 * Returns:
 *    None
 */
void UserInterface::getUniqueEmail(string& email)
{
	bool isEmailUnique = (m_controller->isEmailUnique(email) == Enums::ProcessStatus::SUCCESS) ? true : false;
	while (!isEmailUnique)
	{
		util::readValueWithRetry(email, "Email already exists!. Please enter again: ");
		util::isEmailValid(email);
		if (m_controller->isEmailUnique(email) == Enums::ProcessStatus::SUCCESS)
		{
			isEmailUnique = true;
		}
	}
}

/*
 * Function: getUniquePhoneNumber
 * Description: Ensures that the provided phone number is unique by validating
 *              against existing users in the system. Prompts the user to re‑enter
 *              the phone number until a unique and valid number is provided.
 * Parameters:
 *    phoneNumber (std::string&) - Reference to the variable storing the user's phone number
 * Returns:
 *    None
 */
void UserInterface::getUniquePhoneNumber(string& phoneNumber)
{
	bool isPhoneNumberUnique = (m_controller->isPhoneNumberUnique(phoneNumber) == Enums::ProcessStatus::SUCCESS) ? true : false;
	while (!isPhoneNumberUnique)
	{
		util::readValueWithRetry(phoneNumber, "Phone number already exists!. Please enter again: ");
		util::isPhoneNumberValid(phoneNumber);
		if (m_controller->isPhoneNumberUnique(phoneNumber) == Enums::ProcessStatus::SUCCESS)
		{
			isPhoneNumberUnique = true;
		}
	}
}

/*
 * Function: updateSeatLayout
 * Description: Prompts the user to enter the number of rows and columns for a
 *              given screen’s seating layout. Passes the update request to
 *              the Controller
 * Parameters:
 *    screen  - A pointer to the Screen object whose seat layout is to be updated
 *    rows    - The number of rows in the seating layout
 *    columns - The number of columns in the seating layout
 * Returns:
 *    None
 */
void UserInterface::updateSeatLayout(const string& selectedScreenId, int rows, int columns, double amount)
{
	util::readValueWithRetry(rows, "Enter Number of Rows: ");
	util::readValueWithRetry(columns, "Enter Number of Columns: ");
	if (m_controller->updateSeatLayout(selectedScreenId, rows, columns, amount) == Enums::ProcessStatus::SUCCESS)
	{
		cout << "Seat Layout Updated Successfully" << endl;
	}
	else
	{
		cout << "Failed! Seat Layout could not be updated" << endl;
	}
}

/*
 * Function: handleUserDetailsInput
 * Description: Collects and validates user details including username, email,
 *              password, and phone number. Ensures uniqueness of email and
 *              phone number before proceeding with registration.
 * Parameters:
 *    userName (string&) - Reference to the variable storing the user's name
 *    email (string&) - Reference to the variable storing the user's email
 *    password (string&) - Reference to the variable storing the user's password
 *    phoneNumber (string&) - Reference to the variable storing the user's phone number
 * Returns:
 *    None
 */
void UserInterface::handleUserDetailsInput(string& userName, string& email, string& password, string& phoneNumber)
{
	util::readValueWithRetry(userName, "Enter user name: ");
	util::readValueWithRetry(email, "Enter email: ");
	util::isEmailValid(email);
	getUniqueEmail(email);
	util::readValueWithRetry(password, "Enter password: ");
	util::isPasswordValid(password);
	util::readValueWithRetry(phoneNumber, "Enter phone number: ");
	util::isPhoneNumberValid(phoneNumber);
	getUniquePhoneNumber(phoneNumber);
}

/*
 * Function: isValidTime
 * Description: Validates whether the given hour and minute represent a valid time.
 * Parameters:
 *    hour (int) - Hour value (0–23)
 *    minute (int) - Minute value (0–59)
 * Returns:
 *    bool - True if valid, false otherwise
 */
bool UserInterface::isValidTime(int hour, int minute)
{
	return (hour >= 0 && hour < 24 && minute >= 0 && minute < 60);
}

/*
 * Function: isValidDate
 * Description: Validates whether the given date is valid, including leap year handling.
 * Parameters:
 *    year (int) - Year value
 *    month (int) - Month value
 *    day (int) - Day value
 * Returns:
 *    bool - True if valid, false otherwise
 */
bool UserInterface::isValidDate(int year, int month, int day)
{
	if (year < 1900 || month < 1 || month > 12 || day < 1)
	{
		return false;
	}
	int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
		daysInMonth[1] = 29;
	}
	return day <= daysInMonth[month - 1];
}

/*
 * Function: getValidTime
 * Description: Prompts the user until a valid time is entered.
 * Parameters:
 *    hour (int&) - Reference to store valid hour
 *    minute (int&) - Reference to store valid minute
 * Returns:
 *    void
 */
void UserInterface::getValidTime(int& hour, int& minute)
{
	bool isTimeValid = isValidTime(hour, minute);
	while (!isTimeValid)
	{
		util::readValueWithRetry(hour, "Please enter a valid time (Enter Hour) : ");
		util::readValueWithRetry(minute, "Please enter a valid time (Enter Minutes) : ");
		isTimeValid = isValidTime(hour, minute);
	}
}

/*
 * Function: getValidDate
 * Description: Prompts the user until a valid date is entered.
 * Parameters:
 *    year (int&) - Reference to store valid year
 *    month (int&) - Reference to store valid month
 *    day (int&) - Reference to store valid day
 * Returns:
 *    void
 */
void UserInterface::getValidDate(int& year, int& month, int& day)
{
	bool isDateValid = isValidDate(year, month, day);
	while (!isDateValid)
	{
		util::readValueWithRetry(year, "Please enter a valid date (YYYY) : ");
		util::readValueWithRetry(month, "Please enter a valid month (MM) : ");
		util::readValueWithRetry(day, "Please enter a valid day (DD) : ");
		isDateValid = isValidDate(year, month, day);
	}
}

/*
 * Function: isFutureDateTime
 * Description: Checks whether the given date and time occur in the future.
 * Parameters:
 *    year (int) - Year value
 *    month (int) - Month value
 *    day (int) - Day value
 *    hour (int) - Hour value
 *    minute (int) - Minute value
 * Returns:
 *    bool - True if the datetime is in the future, false otherwise
 */
bool UserInterface::isFutureDateTime(int year, int month, int day, int hour, int minute)
{
	time_t inputTime = util::createTime(year, month, day, hour, minute);
	if (inputTime == -1)
	{
		return false;
	}
	time_t currentTime = time(0);
	return difftime(inputTime, currentTime) > 0;
}

/*
 * Function: addShow
 * Description: Handles user interaction to add a new show, including
 *              input collection, validation, and invoking controller logic.
 * Parameters: None
 * Returns:
 *    void
 */
void UserInterface::addShow()
{
	string showId;
	string movieId, screenId, theatreId;
	int year, month, day;
	int startTimeHour, startTimeMinute;
	Enums::ProcessStatus movieDisplayStatus = displayMoviesInTheatre(theatreId);
	if (movieDisplayStatus == Enums::ProcessStatus::FAILED)
	{
		cout << "No movies present in theatre" << endl;
		util::pressEnterToContinue();
		util::clear();
		return;
	}
	util::readValueWithRetry(movieId, "Enter Movie ID: ");
	Enums::ProcessStatus isMoviePresent = m_controller->isMovieInTheatre(movieId, theatreId);
	if (isMoviePresent == Enums::ProcessStatus::FAILED)
	{
		cout << "Movie is not present in theatre" << endl;
		util::pressEnterToContinue();
		util::clear();
		return;
	}
	const vector<const Screen*> screens = m_controller->getScreensFromTheatre(theatreId);
	if (screens.empty())
	{
		cout << "No screens available to add show to! " << endl;
		util::pressEnterToContinue();
		util::clear();
		return;
	}
	if (!getScreenId(screens, screenId))
	{
		cout << "Invalid screen ID!" << endl;
		util::pressEnterToContinue();
		util::clear();
		return;
	}
	util::readValueWithRetry(year, "Enter date (YYYY): ");
	util::readValueWithRetry(month, "Enter month (MM): ");
	util::readValueWithRetry(day, "Enter day (DD): ");
	getValidDate(year, month, day);
	cout << "Enter start time (HH MM): \n";
	util::readValueWithRetry(startTimeHour, "Enter a valid time (Enter Hour) : ");
	util::readValueWithRetry(startTimeMinute, "Enter a valid time (Enter Minutes) : ");
	getValidTime(startTimeHour, startTimeMinute);
	if (!isFutureDateTime(year, month, day, startTimeHour, startTimeMinute))
	{
		cout << "Cannot add a show with a past time/date!" << endl;
		util::pressEnterToContinue();
		util::clear();
		return;
	}
	if (m_controller->isShowTimeConflicting(movieId, screenId, year, month, day, startTimeHour, startTimeMinute) == Enums::ProcessStatus::FAILED)
	{
		cout << "Cannot add show as it conflicts with the time of another show!" << endl;
		util::pressEnterToContinue();
		util::clear();
		return;
	}
	if (m_controller->addShow(movieId, screenId, year, month, day, startTimeHour, startTimeMinute) == Enums::ProcessStatus::SUCCESS)
	{
		cout << "Show added successfully!" << endl;
		util::pressEnterToContinue();
	}
}

/*
 * Function: createUser
 * Description: Allows an Admin to register a new user directly by selecting
 *              the role (Customer, Theatre Owner, or Admin) and entering
 *              user details.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::createUser()
{
	string userName, email, password, phoneNumber;
	Enums::UserType userType = Enums::UserType::CUSTOMER;
	unsigned short choice;
	userTypesAdminMenu();
	util::readValueWithRetry(choice, "Enter a choice: ");
	switch (choice)
	{
		case 1:
			userType = Enums::UserType::CUSTOMER;
			break;
		case 2:
			userType = Enums::UserType::THEATRE_OWNER;
			break;
		case 3:
			userType = Enums::UserType::ADMIN;
			break;
		default:
			cout << "Invalid Choice. Please Try again!" << endl;
			util::pressEnterToContinue();
			util::clear();
			return;
	}
	handleUserDetailsInput(userName, email, password, phoneNumber);
	Enums::ProcessStatus status = m_controller->createUser(userName, email, password, phoneNumber, userType);
	if (status == Enums::ProcessStatus::SUCCESS)
	{
		cout << "User registered successfully!" << endl;
		util::pressEnterToContinue();
		util::clear();
	}
	else if (status == Enums::ProcessStatus::EMAIL_ALREADY_EXISTS)
	{
		cout << "Email Already exists! Registration failed." << endl;
		util::pressEnterToContinue();
		util::clear();
	}
	else if (status == Enums::ProcessStatus::PHONE_NUMBER_ALREADY_EXISTS)
	{
		cout << "Phone Number Already exists! Registration failed." << endl;
		util::pressEnterToContinue();
		util::clear();
	}
	else
	{
		cout << "User could not be registered!" << endl;
		util::pressEnterToContinue();
		util::clear();
	}
}

/*
 * Function: userTypesAdminMenu
 * Description: Displays the list of available user types (Customer, Theatre Owner,
 *              Admin) for Admin registration.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::userTypesAdminMenu()
{
	cout << "-------------------User Type List-------------------" << endl;
	cout << "1. Customer" << endl;
	cout << "2. Theatre Owner" << endl;
	cout << "3. Admin" << endl;
	cout << "----------------------" << endl;
}

/*
 * Function: updateUserDetails
 * Description: Provides a menu for the authenticated user to update
 *              their details (username, email, phone number). Prompts the user
 *              for each field individually, validates input, and passes the
 *              update request to the Controller.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::updateUserDetails()
{
	bool condition = true;
	while (condition)
	{
		unsigned short choice;
		string input;
		Enums::ProcessStatus result = Enums::ProcessStatus::FAILED;
		viewProfile();
		updateUserDetailsMenu();
		util::readValueWithRetry(choice, "Enter a choice: ");
		switch (choice)
		{
			case 1:
				util::readValueWithRetry(input, "Enter username: ");
				result = m_controller->setAuthenticatedUserUserName(input);
				break;
			case 2:
				util::readValueWithRetry(input, "Enter email: ");
				util::isEmailValid(input);
				getUniqueEmail(input);
				result = m_controller->setAuthenticatedUserEmail(input);
				break;
			case 3:
				util::readValueWithRetry(input, "Enter phoneNumber: ");
				util::isPhoneNumberValid(input);
				getUniquePhoneNumber(input);
				result = m_controller->setAuthenticatedUserPhoneNumber(input);
				break;
			case 4:
				condition = false;
				break;
			default:
				cout << "Enter a valid option!" << endl;
				util::pressEnterToContinue();
				util::clear();
				break;
		}
		if (choice >= 1 && choice <= 3)
		{
			if (result == Enums::ProcessStatus::SUCCESS)
			{
				cout << "user Details Updated Successfully" << endl;
			}
			else
			{
				cout << "failed, could not update user details!" << endl;
			}
			util::pressEnterToContinue();
			util::clear();
		}
	}
	util::pressEnterToContinue();
	util::clear();
}

/*
 * Function: updateUserDetailsMenu
 * Description: Displays the update user details menu options (username, email,
 *              phone number, exit) and prompts the user to select an option.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::updateUserDetailsMenu()
{
	cout << "<--- User Details Menu---->" << endl;
	cout << "1. User Name\n2. Email\n3. Phone Number\n4. Exit\n-----------------\n";
}

/*
 * Function: deactivateUser
 * Description: Displays all users, prompts the Admin to enter a User ID,
 *              and attempts to deactivate the selected user. Provides feedback on
 *              whether the deactivation was successful or failed.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::deactivateUser()
{
	string userId;
	viewAllUsers();
	util::readValueWithRetry(userId, "Enter the User ID: ");
	Enums::ProcessStatus result = m_controller->deactivateUser(userId);
	if (result == Enums::ProcessStatus::SUCCESS)
	{
		cout << "USER " << userId << " deactivated successfully" << endl;
	}
	else
	{
		cout << "Invalid User Id, Try again!" << endl;
	}
	util::pressEnterToContinue();
	util::clear();
}

/*
* Function Name : viewNotifications
* Description   : Displays unread notifications for the authenticated user in batches.
* Parameters    : None
* Return Type   : void
*/
void UserInterface::viewNotifications()
{
	bool condition = true;
	unsigned short choice = 1;
	int count = 0;
	while (condition)
	{
		int remainingUnreadCount = 0;
		const vector<string> notifications = m_controller->getUnreadNotifications(5, remainingUnreadCount);
		if (notifications.empty())
		{
			cout << "No unread notifications." << endl;
			util::pressEnterToContinue();
			return;
		}
		cout << endl;
		cout << "========= Notifications =========" << endl << endl;
		for (vector<string>::const_iterator iterator = notifications.begin(); iterator != notifications.end(); ++iterator)
		{
			cout << ++count << ". " << (*iterator) << endl;
		}
		if (remainingUnreadCount > 0)
		{
			cout << remainingUnreadCount << " unread notifications remaining." << endl;
			util::readValueWithRetry(choice, "\n1. View More\n2. Exit\n");
			if (choice == 2)
			{
				condition = false;
			}
		}
		else
		{
			cout << "No more unread notifications." << endl;
			break;
		}
	}
	util::pressEnterToContinue();
}

/*
 * Function: viewInactiveUsers
 * Description: Retrieves all inactive users from the Controller and displays
 *              them in a tabular format on the console.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::viewInactiveUsers()
{
	const vector<const User*> users = m_controller->getInactiveUsers();
	if (users.empty())
	{
		cout << "No users found!" << endl;
	}
	cout << left
		<< setw(14) << "User ID"
		<< setw(22) << "Name"
		<< setw(32) << "Email"
		<< setw(20) << "Password"
		<< setw(16) << "Role"
		<< endl;
	cout << string(104, '-') << endl;
	for (vector<const User*>::const_iterator iterator = users.begin(); iterator != users.end(); ++iterator)
	{
		cout << left
			<< setw(14) << (*iterator)->getUserId()
			<< setw(22) << (*iterator)->getUserName()
			<< setw(32) << (*iterator)->getEmail()
			<< setw(20) << (*iterator)->getPassword()
			<< setw(16) << Enums::getUserTypeString((*iterator)->getUserType())
			<< endl;
	}
	util::pressEnterToContinue();
	util::clear();
}

/*
 * Function: handleMovieDetailsInput
 * Description: Validates whether the provided movie details represent a unique movie in the system.
 * Parameters:
 *    title    - Title of the movie
 *    language - Language of the movie
 *    genre    - Genre of the movie
 *    duration - Duration of the movie in minutes
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the movie is unique,
 *    Enums::ProcessStatus::FAILED if a duplicate exists
 */
Enums::ProcessStatus UserInterface::handleMovieDetailsInput(const string& title, const string& language, const string& genre, const int duration)
{
	return m_controller->isMovieUnique(title, language, genre, duration);
}

/*
 * Function: viewSeatLayout
 * Description: Displays the seating grid of a given screen.
 * Parameters:
 *    Screen* - Target screen
 * Returns:
 *    None
 */
void UserInterface::viewSeatLayout(const string& selectedScreenId)
{
	const vector<vector<Seat*>>& seatGrid = m_controller->getSeatLayout(selectedScreenId);
	cout << "<------ Seat Grid ------>" << endl << endl;
	for (vector<vector<Seat*>>::const_iterator iteratorOne = seatGrid.begin(); iteratorOne != seatGrid.end(); ++iteratorOne)
	{
		for (vector<Seat*>::const_iterator iteratorTwo = (*iteratorOne).begin(); iteratorTwo != (*iteratorOne).end(); ++iteratorTwo)
		{
			if (!(*iteratorTwo))
			{
				cout << "[NA]\t" << " ";
				continue;
			}
			if ((*iteratorTwo)->getSeatStatus() == Enums::SeatStatus::AVAILABLE)
			{
				cout << (*iteratorTwo)->getSeatId() << "-[A]\t" << " ";
			}
			else if ((*iteratorTwo)->getSeatStatus() == Enums::SeatStatus::BOOKED)
			{
				cout << (*iteratorTwo)->getSeatId() << "-[B]\t" << " ";
			}
			else if ((*iteratorTwo)->getSeatStatus() == Enums::SeatStatus::BLOCKED)
			{
				cout << (*iteratorTwo)->getSeatId() << "-[D]\t" << " ";
			}
		}
		cout << endl;
	}
	cout << endl;
	cout << "[A] - Available  [B] - Booked  [D] - Blocked" << endl;
}

/*
 * Function: getMoviesByTitleInput
 * Description: Prompts the user to enter a movie title and retrieves all movies
 *              matching that title from the Controller. Acts as a user input
 *              handler for initiating movie search operations.
 * Parameters: None
 * Returns:
 *    A vector of constant Movie pointers representing the movies that match
 *    the entered title. Returns an empty vector if no movies are found.
 */
const vector<const Movie*> UserInterface::getMoviesByTitleInput()
{
	string title;
	util::readValueWithRetry(title, "\nEnter the movie title: ");
	return m_controller->searchMovieByTitle(title);
}

/*
 * Function: validateMovieIdInput
 * Description: Prompts the user to enter a movie ID and validates it against
 *              the list of available movie IDs derived from the provided movies.
 *              Ensures that the entered ID corresponds to a valid movie in the list.
 * Parameters:
 *    movies  - A vector of Movie pointers representing the movies to validate against.
 *    movieId - Reference string to store the user-entered movie ID.
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the entered movie ID is valid.
 *    Enums::ProcessStatus::FAILED if the entered movie ID is invalid.
 */
Enums::ProcessStatus UserInterface::validateMovieIdInput(const vector<const Movie*>& movies, string& movieId)
{
	const vector<string> movieIdList = getMovieIdFromList(movies);
	util::readValueWithRetry(movieId, "\nEnter the Movie ID: ");
	Enums::ProcessStatus status = checkMovieIdIsValid(movieId, movieIdList);
	if (status == Enums::ProcessStatus::SUCCESS)
	{
		return Enums::ProcessStatus::SUCCESS;
	}
	return Enums::ProcessStatus::FAILED;
}

/*
 * Function: editMovieDetails
 * Description: Provides an interactive menu-driven workflow for editing movie details.
 *              Allows the user to update attributes such as title, language, genre,
 *              and duration for a selected movie. Continuously prompts the user until
 *              they choose to exit by entering 0.
 * Parameters:
 *    movieId      - Unique identifier of the movie to be edited.
 *    currentMovie - Pointer to the current Movie object being updated.
 * Returns: None
 */
void UserInterface::editMovieDetails(const string& movieId, const Movie* currentMovie)
{
	unsigned short choice = 1;
	string title, genre, language;
	int duration;
	while (choice != 0)
	{
		displayEditMovieMenu();
		util::readValueWithRetry(choice, "\nEnter which details want to edit: ");
		switch (choice)
		{
			case 1:
				util::readValueWithRetry(title, "\nEnter the new Title: ");
				changeMovieTitle(movieId, title, currentMovie);
				break;
			case 2:
				util::readValueWithRetry(language, "\nEnter the new Language: ");
				changeMovieLanguage(movieId, language, currentMovie);
				break;
			case 3:
				util::readValueWithRetry(genre, "\nEnter the new Genre: ");
				changeMovieGenre(movieId, genre, currentMovie);
				break;
			case 4:
				util::readValueWithRetry(duration, "\nEnter the new Duration: ");
				changeMovieDuration(movieId, duration, currentMovie);
				break;
		}
	}
}

/*
 * Function: updateMovie
 * Description: Allows the user to update details of an existing movie by searching with its title,
 *              validating the movie ID, and presenting an edit menu for modifying attributes.
 * Parameters:
 *    None (reads input directly from the user)
 * Returns:
 *    None
 */
void UserInterface::updateMovie()
{
	const vector<const Movie*> movies = getMoviesByTitleInput();
	if (movies.empty())
	{
		cout << "\nNo movies found with given title.";
		return;
	}
	displayMovie(movies);
	string movieId;
	if (validateMovieIdInput(movies, movieId) == Enums::ProcessStatus::FAILED)
	{
		cout << "\nEnter valid Movie Id from list!";
		return;
	}
	displayCurrentMovieDetails(movieId, movies);
	const Movie* currentMovie = getCurrentMovie(movieId, movies);
	editMovieDetails(movieId, currentMovie);
}

/*
 * Function: getMovieIdFromList
 * Description: Extracts and returns a list of movie IDs from the given movie collection.
 * Parameters:
 *    movies - Vector of constant Movie pointers
 * Returns:
 *    A vector of strings containing movie IDs
 */
const vector<string> UserInterface::getMovieIdFromList(const vector<const Movie*>& movies)
{
	vector<string> movieIds;
	for (vector<const Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		const Movie* movie = *iterator;
		if (movie)
		{
			movieIds.push_back(movie->getMovieId());
		}
	}
	return movieIds;
}

/*
 * Function: checkMovieIdIsValid
 * Description: Validates whether the provided movie ID exists within the given list of IDs.
 * Parameters:
 *    movieId     - Movie ID to validate
 *    movieIdList - Vector of valid movie IDs
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the ID is valid,
 *    Enums::ProcessStatus::FAILED otherwise
 */
Enums::ProcessStatus UserInterface::checkMovieIdIsValid(const string& movieId, const vector<string>& movieIdList)
{
	bool flag = false;
	for (vector<string>::const_iterator iterator = movieIdList.begin(); iterator != movieIdList.end(); ++iterator)
	{
		if (*iterator == movieId)
		{
			flag = true;
		}
	}
	if (flag)
	{
		return Enums::ProcessStatus::SUCCESS;
	}
	return Enums::ProcessStatus::FAILED;
}

/*
 * Function: displayCurrentMovieDetails
 * Description: Displays the details of a specific movie identified by its ID.
 * Parameters:
 *    movieId - Movie ID to display
 *    movies  - Vector of constant Movie pointers
 * Returns:
 *    None
 */
void UserInterface::displayCurrentMovieDetails(const string& movieId, const vector<const Movie*>& movies)
{
	cout << string(78, '-') << endl;
	cout << left
		<< setw(14) << "ID"
		<< setw(22) << "Title"
		<< setw(14) << "Language"
		<< setw(18) << "Genre"
		<< setw(10) << "Duration"
		<< endl;
	cout << string(78, '-') << endl;
	for (vector<const Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		if ((*iterator)->getMovieId() == movieId)
		{
			cout << left
				<< setw(14) << (*iterator)->getMovieId()
				<< setw(22) << (*iterator)->getTitle()
				<< setw(14) << (*iterator)->getLanguage()
				<< setw(18) << (*iterator)->getGenre()
				<< setw(10) << (*iterator)->getDuration()
				<< endl;
		}
	}
	util::pressEnterToContinue();
}

/*
 * Function: getCurrentMovie
 * Description: Retrieves a specific movie object from the list based on its ID.
 * Parameters:
 *    movieId - Movie ID to search for
 *    movies  - Vector of constant Movie pointers
 * Returns:
 *    Pointer to the Movie object if found, otherwise nullptr
 */
const Movie* UserInterface::getCurrentMovie(const string& movieId, const vector<const Movie*>& movies)
{
	for (vector<const Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		if ((*iterator)->getMovieId() == movieId)
		{
			return *iterator;
		}
	}
	return nullptr;
}

/*
 * Function: displayEditMovieMenu
 * Description: Displays the edit menu options for modifying movie details.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::displayEditMovieMenu()
{
	cout << "\n\n1.Title";
	cout << "\n2.Language";
	cout << "\n3.Genre";
	cout << "\n4.Duration";
	cout << "\n0.Exit";
	cout << "\n------------------" << endl;
}

/*
 * Function: changeMovieTitle
 * Description: Updates the title of a movie after validating uniqueness.
 * Parameters:
 *    movieId     - Unique identifier of the movie
 *    title       - New title to set
 *    currentMovie - Pointer to the current Movie object
 * Returns:
 *    None
 */
void UserInterface::changeMovieTitle(const string& movieId, const string& title, const Movie*& currentMovie)
{
	if (Enums::ProcessStatus::SUCCESS == handleMovieDetailsInput(title, currentMovie->getLanguage(), currentMovie->getGenre(), currentMovie->getDuration()))
	{
		if (m_controller->setMovieTitleByID(movieId, title) == Enums::ProcessStatus::SUCCESS)
		{
			cout << "\nTitle has been updated!.";
		}
		else
		{
			cout << "\nTitle can't been Update!.";
		}
	}
	else
	{
		cout << "\nMovie already exist!" << endl;
		return;
	}
}

/*
 * Function: changeMovieLanguage
 * Description: Updates the language of a movie after validating uniqueness.
 * Parameters:
 *    movieId     - Unique identifier of the movie
 *    language    - New language to set
 *    currentMovie - Pointer to the current Movie object
 * Returns:
 *    None
 */
void UserInterface::changeMovieLanguage(const string& movieId, const string& language, const Movie*& currentMovie)
{
	if (Enums::ProcessStatus::SUCCESS == handleMovieDetailsInput(currentMovie->getTitle(), language, currentMovie->getGenre(), currentMovie->getDuration()))
	{
		if ((m_controller->setMovieLanguageByID(movieId, language) == Enums::ProcessStatus::SUCCESS))
		{
			cout << "\nLanguage has been updated!.";
		}
		else
		{
			cout << "\nLanguage can't been update!.";
		}
	}
	else
	{
		cout << "\nMovie already exist!" << endl;
		return;
	}
}

/*
 * Function: viewTheatreDetails
 * Description: Retrieves and displays details of theatres owned by the current user.
 *              Queries the Controller for theatres associated with the current owner
 *              and presents their information if available. If no theatres are found,
 *              informs the user accordingly.
 * Parameters: None
 * Returns: None
 */
void UserInterface::viewTheatreDetails()
{
	const vector<const Theatre*> theatres = m_controller->getCurrentOwnerTheatres();
	if (!theatres.empty())
	{
		displayTheatreDetails(theatres);
	}
	else
	{
		cout << "No theatres found for current owner" << endl;
		util::pressEnterToContinue();
	}
}

/*
 * Function: displayTheatreDetails
 * Description: Displays detailed information about a list of theatres in a
 *              formatted tabular view. Outputs theatre attributes such as
 *              ID, name, city, address, contact phone, and current status.
 * Parameters:
 *    theatres - A vector of Theatre pointers representing the theatres
 *               whose details are to be displayed.
 * Returns: None
 */
void UserInterface::displayTheatreDetails(const vector<const Theatre*>& theatres)
{
	if (theatres.empty())
	{
		cout << "No theatres available." << endl;
		util::pressEnterToContinue();
		return;
	}
	cout << string(112, '-') << endl;
	cout << left
		<< setw(16) << "ID"
		<< setw(22) << "Name"
		<< setw(16) << "City"
		<< setw(28) << "Address"
		<< setw(16) << "Contact"
		<< setw(14) << "Status"
		<< endl;
	cout << string(112, '-') << endl;
	for (vector<const Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
	{
		if (*iterator)
		{
			cout << left
				<< setw(16) << (*iterator)->getTheatreId()
				<< setw(22) << (*iterator)->getName()
				<< setw(16) << (*iterator)->getCity()
				<< setw(28) << (*iterator)->getAddress()
				<< setw(16) << (*iterator)->getTheatrePhoneNumber()
				<< setw(14) << Enums::getTheatreStatusString((*iterator)->getStatus())
				<< endl;
		}
	}
	util::pressEnterToContinue();
}

/*
 * Function: changeMovieGenre
 * Description: Updates the genre of a movie after validating uniqueness.
 * Parameters:
 *    movieId     - Unique identifier of the movie
 *    genre       - New genre to set
 *    currentMovie - Pointer to the current Movie object
 * Returns:
 *    None
 */
void UserInterface::changeMovieGenre(const string& movieId, const string& genre, const Movie*& currentMovie)
{
	if (Enums::ProcessStatus::SUCCESS == handleMovieDetailsInput(currentMovie->getTitle(), currentMovie->getLanguage(), genre, currentMovie->getDuration()))
	{
		if ((m_controller->setMovieGenreByID(movieId, genre) == Enums::ProcessStatus::SUCCESS))
		{
			cout << "\nGenre has been updated!.";
		}
		else
		{
			cout << "\nGenre can't updated!.";
		}
	}
	else
	{
		cout << "\nMovie already exist!" << endl;
		return;
	}
}

/*
 * Function: changeMovieDuration
 * Description: Updates the duration of a movie after validating uniqueness.
 * Parameters:
 *    movieId     - Unique identifier of the movie
 *    duration    - New duration (in minutes) to set
 *    currentMovie - Pointer to the current Movie object
 * Returns:
 *    None
 */
void UserInterface::changeMovieDuration(const string& movieId, int& duration, const Movie*& currentMovie)
{
	if (Enums::ProcessStatus::SUCCESS == handleMovieDetailsInput(currentMovie->getTitle(), currentMovie->getLanguage(), currentMovie->getGenre(), duration))
	{
		if ((m_controller->setMovieDurationByID(movieId, duration) == Enums::ProcessStatus::SUCCESS))
		{
			cout << "\nDuration has been updated!.";
		}
		else
		{
			cout << "\nDuration can't updated!.";
		}
	}
	else
	{
		cout << "\nMovie already exist!" << endl;
		return;
	}
}

/*
 * Function: UserInterface::addMovie
 * Description: Collects movie details from the user, validates uniqueness, and adds the movie to the system.
 * Parameters:
 *    None (reads input directly from the user)
 * Returns:
 *    None
 */
void UserInterface::addMovie()
{
	string title, language, genre;
	int duration;
	util::readValueWithRetry(title, "\nEnter the Movie Title: ");
	util::readValueWithRetry(language, "\nLanguage             : ");
	util::readValueWithRetry(genre, "\nGenre                :");
	util::readValueWithRetry(duration, "\nDuration(in minutes) :");
	util::isMovieDurationValid(duration);
	if (Enums::ProcessStatus::SUCCESS == handleMovieDetailsInput(title, language, genre, duration))
	{
		if (m_controller->addMovie(title, language, genre, duration) == Enums::ProcessStatus::SUCCESS)
		{
			cout << "Movie Successfully Added" << endl;
		}
		else
		{
			cout << "Movie could not be added!" << endl;
		}
	}
	else
	{
		cout << "\nThe movie already exists!. Please try another. \n";
	}
	util::pressEnterToContinue();
}

/*
 * Function: reactivateUser
 * Description: Displays all inactive users, prompts the Admin to enter a User ID,
 *              and to activate the selected user. Provides feedback on
 *              whether the activation was successful or failed.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::reactivateUser()
{
	string userId;
	viewInactiveUsers();
	util::readValueWithRetry(userId, "Enter the User ID: ");
	Enums::ProcessStatus result = m_controller->reactivateUser(userId);
	if (result == Enums::ProcessStatus::SUCCESS)
	{
		cout << "USER " << userId << " activated successfully" << endl;
	}
	else
	{
		cout << "Invalid User Id, Try again!" << endl;
	}
	util::pressEnterToContinue();
	util::clear();
}

/*
 * Function: UserInterface::viewLogsByType
 * Description: Displays logs filtered by type. Prompts the user to select a log type
 *              and retrieves matching logs from the controller. Each log entry is
 *              printed to the console.
 * Parameters: None
 * Returns: None
 */
void UserInterface::viewLogsByType()
{
	unsigned short choice;
	logsTypeMenu();
	util::readValueWithRetry(choice, "Select a type: ");
	vector<const Log*> logs;
	switch (choice)
	{
		case 1:
			logs = m_controller->getLogsByType(Enums::LogType::SYSTEM_ACTIVITY);
			break;
		case 2:
			logs = m_controller->getLogsByType(Enums::LogType::ERROR_LOG);
			break;
		case 3:
			logs = m_controller->getLogsByType(Enums::LogType::UNKNOWN);
			break;
		default:
			cout << "Invalid choice. Please try again!" << endl;
			util::pressEnterToContinue();
			util::clear();
			break;
	}
	util::clear();
	if (logs.empty())
	{
		cout << "No logs available!" << endl;
	}
	for (vector<const Log*>::const_iterator iterator = logs.begin(); iterator != logs.end(); ++iterator)
	{
		cout << (*iterator)->toString() << endl;
	}
	util::pressEnterToContinue();
}

/*
 * Function: logsTypeMenu
 * Description: Displays the menu of available log types for user selection.
 * Parameters: None
 * Returns: None
 */
void UserInterface::logsTypeMenu()
{
	util::clear();
	cout << "----------------Log Types Menu---------------" << endl;
	cout << "1. " << Enums::getLogTypeString(Enums::LogType::SYSTEM_ACTIVITY) << endl;
	cout << "2. " << Enums::getLogTypeString(Enums::LogType::ERROR_LOG) << endl;
	cout << "3. " << Enums::getLogTypeString(Enums::LogType::UNKNOWN) << endl;
	cout << "--------------------------" << endl;
}

/*
 * Function: viewAllUsers
 * Description: Displays all registered users on the console.
 *              Shows user details including User ID, Name, Email, Password,
 *              Role, and Status.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::viewAllUsers()
{
	const vector<const User*>& users = m_controller->getActiveUsers();
	if (users.empty())
	{
		cout << "No users found!" << endl;
	}
	else
	{
		cout << left
			<< setw(14) << "User ID"
			<< setw(22) << "Name"
			<< setw(32) << "Email"
			<< setw(20) << "Password"
			<< setw(16) << "Role"
			<< endl;
		cout << string(104, '-') << endl;
		for (vector<const User*>::const_iterator iterator = users.begin(); iterator != users.end(); ++iterator)
		{
			cout << left
				<< setw(14) << (*iterator)->getUserId()
				<< setw(22) << (*iterator)->getUserName()
				<< setw(32) << (*iterator)->getEmail()
				<< setw(20) << (*iterator)->getPassword()
				<< setw(16) << Enums::getUserTypeString((*iterator)->getUserType())
				<< endl;
		}
	}
	util::pressEnterToContinue();
	util::clear();
}

/*
 * Function: viewProfile
 * Description: Retrieves the currently authenticated user from the Controller
 *              and displays their profile details.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::viewProfile()
{
	const User* const currentUser = m_controller->getAuthenticatedUser();
	cout << "User Id: " << currentUser->getUserId() << endl;
	cout << "Name : " << currentUser->getUserName() << endl;
	cout << "Email: " << currentUser->getEmail() << endl;
	cout << "Phone Number: " << currentUser->getPhoneNumber() << endl;
	util::pressEnterToContinue();
	util::clear();
}

/*
 * Function: changePassword
 * Description: Prompts the currently authenticated user to change their password.
 *              Requests the current password and a new password from the user,
 *              validates the new password, and pass the change request to
 *              the Controller. Provides feedback on whether the password change
 *              was successful or failed due to a mismatch.
 * Parameters:
 *    currentPassword (string) - The current password entered by the user
 *    newPassword (string)    - The new password entered by the user
 * Returns:
 *    None
 */
void UserInterface::changePassword()
{
	string currentPassword, newPassword;
	util::readValueWithRetry(currentPassword, "Enter the current password: ");
	util::readValueWithRetry(newPassword, "Enter the new Password: ");
	util::isPasswordValid(newPassword);
	Enums::ProcessStatus result = m_controller->changePassword(currentPassword, newPassword);
	if (result == Enums::ProcessStatus::SUCCESS)
	{
		cout << "Password changed successfully." << endl;
	}
	else
	{
		cout << "Password cannot be changed." << endl;
	}
	util::pressEnterToContinue();
	util::clear();
}

/*
 * Function: displayAllMovies
 * Description: Retrieves and displays all active movies in the system. If no active movies exist,
 *              informs the user accordingly.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::displayAllMovies()
{
	const vector<const Movie*> movies = m_controller->getAllActiveMovies();
	if (!(movies.empty()))
	{
		displayMovie(movies);
	}
	else
	{
		cout << "\nNo current movies!.";
	}
}

/*
 * Function: DeactivateMovie
 * Description: Allows the user to deactivate a movie in the system by searching with its title,
 *              validating the movie ID, and updating its status to INACTIVE through the Controller.
 * Parameters:
 *    None (reads input directly from the user)
 * Returns:
 *    None
 */
void UserInterface::deactivateMovie()
{
	const vector<const Movie*> movies = m_controller->getAllActiveMovies();
	if (movies.empty())
	{
		return;
	}
	displayMovie(movies);
	string title, movieId;
	util::readValueWithRetry(title, "\nEnter the movie title: ");
	if (!movies.empty())
	{
		const vector<string> movieIdList = getMovieIdFromList(movies);
		util::readValueWithRetry(movieId, "\nEnter the Movie ID: ");
		if (checkMovieIdIsValid(movieId, movieIdList) == Enums::ProcessStatus::SUCCESS)
		{
			if (m_controller->deactivateMovie(movieId) == Enums::ProcessStatus::SUCCESS)
			{
				cout << "\nMovie status has changed to Deactivate!.";
			}
			else
			{
				cout << "\nCan't change the movie status!.";
			}
		}
		else
		{
			cout << "\nEnter valid Movie Id from list!";
		}
	}
	else
	{
		cout << "\nNo movies with " << title << " name!.";
	}
	util::pressEnterToContinue();
}

/*
 * Function: activateMovie
 * Description: Allows the user to activate a movie in the system by searching with its title,
 *              validating the movie ID, and updating its status to ACTIVE through the Controller.
 * Parameters:
 *    None (reads input directly from the user)
 * Returns:
 *    None
 */
void UserInterface::activateMovie()
{
	if (!displayAllInactiveMovies())
	{
		return;
	}
	string title, movieId;
	util::readValueWithRetry(title, "\nEnter the movie title: ");
	const vector<const Movie*> movies = m_controller->searchDeactivatedMovieByTitle(title);
	if (!movies.empty())
	{
		displayMovie(movies);
		const vector<string> movieIdList = getMovieIdFromList(movies);
		util::readValueWithRetry(movieId, "\nEnter the Movie ID: ");
		if (checkMovieIdIsValid(movieId, movieIdList) == Enums::ProcessStatus::SUCCESS)
		{
			if (m_controller->reactivateMovie(movieId) == Enums::ProcessStatus::SUCCESS)
			{
				cout << "\nMovie status has changed to Activate!.";
			}
			else
			{
				cout << "\nCan't change the movie status!.";
			}
		}
		else
		{
			cout << "\nEnter valid Movie Id from list!";
		}
	}
	else
	{
		cout << "\nNo movies with " << title << " name!.";
	}
	util::pressEnterToContinue();
}

/*
 * Function: searchMovie
 * Description: Allows the user to search for movies by title. Prompts the user
 *              to enter a movie title, retrieves matching movies from the
 *              Controller, and displays the results if found.
 * Parameters: None
 * Returns: None
 */
void UserInterface::searchMovie()
{
	string title;
	util::readValueWithRetry(title, "\nEnter the movie title: ");
	const vector<const Movie*> movies = m_controller->searchMovieByTitle(title);
	if (!movies.empty())
	{
		displayMovie(movies);
	}
	else
	{
		cout << "No movie exists with name " << title << endl;
		util::pressEnterToContinue();
	}
}

/*
 * Function: displayAllInactiveMovies
 * Description: Retrieves and displays all inactive movies in the system. If no inactive movies exist,
 *              informs the user accordingly.
 * Parameters:
 *    None
 * Returns:
 *    bool
 */
bool UserInterface::displayAllInactiveMovies()
{
	const vector<const Movie*> movies = m_controller->getAllInactiveMovies();
	if (movies.empty())
	{
		cout << "\nNo inactive movies!.";
		util::pressEnterToContinue();
		return false;
	}
	displayMovie(movies);
	return true;
}

/*
 * Function: deactivateSeat
 * Description: Deactivates a specific seat in the given screen and shows result.
 * Parameters:
 *    Screen* - Target screen
 *    seatId (string) - Identifier of the seat
 * Returns:
 *    None
 */
void UserInterface::deactivateSeat(const string& selectedScreenId, const string& seatId)
{
	Enums::ProcessStatus status = m_controller->deactivateSeat(selectedScreenId, seatId);
	if (status == Enums::ProcessStatus::SUCCESS)
	{
		cout << "Seat Number " << seatId << " Deactivated!" << endl;
	}
	else
	{
		cout << "Failure! Seat could not be deactivated." << endl;
	}
}

/*
 * Function: reactivateSeat
 * Description: Reactivates a specific seat in the given screen and shows result.
 * Parameters:
 *    screen - Target screen
 *    seatId - Identifier of the seat
 * Returns:
 *    None
 */
void UserInterface::reactivateSeat(const string& selectedScreenId, const string& seatId)
{
	Enums::ProcessStatus status = m_controller->reactivateSeat(selectedScreenId, seatId);
	if (status == Enums::ProcessStatus::SUCCESS)
	{
		cout << "Seat Number " << seatId << " Reactivated!" << endl;
	}
	else
	{
		cout << "Failure! Seat could not be Reactivated." << endl;
	}
}

/*
* Function Name : viewShowSeatLayout
* Description   : Displays the seat layout for a given show.
*                 Iterates through the 2D seat layout matrix retrieved from the controller
*                 and prints each seat. Also displays the seat status codes
*				  (Available, Booked, Reserved, Blocked, Invalid).
* Parameters    :
*                  show - Pointer to the Show object whose seat layout is to be displayed
* Return Type   : void
*/
void UserInterface::viewShowSeatLayout(const Show* show)
{
	const vector<vector<string>> layout = m_controller->getShowSeatLayout(show);
	for (vector<vector<string>>::const_iterator rowIterator = layout.begin(); rowIterator != layout.end(); ++rowIterator)
	{
		for (vector<string>::const_iterator columnIterator = (*rowIterator).begin(); columnIterator != (*rowIterator).end(); ++columnIterator)
		{
			cout << *columnIterator << "\t";
		}
		cout << endl;
	}
	cout << endl;
	cout << "[A] - Available  [B] - Booked  [D] - Blocked  [NA] - Invalid Seat" << endl;
}

/*
* Function Name : addScreen
* Description   : Adds a new screen to the theatre.
* Parameters    :
*                  theatre     - Theatre where the screen is added
*                  name        - Name of the screen
*                  seatRows    - Number of seat rows
*                  seatColumns - Number of seat columns
* Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus UserInterface::addScreen(const string& theatreId, const string& name, int seatRows, int seatColumns, double seatAmount)
{
	Enums::ProcessStatus status = m_controller->addScreen(theatreId, name, seatRows, seatColumns, seatAmount);
	if (status == Enums::ProcessStatus::SUCCESS)
	{
		cout << "Screen with Name : " << name << " Added Successfully." << endl;
		return Enums::ProcessStatus::SUCCESS;
	}
	if (status == Enums::ProcessStatus::FAILED)
	{
		cout << "Failed! Could not add screen." << endl;
	}
	if (status == Enums::ProcessStatus::NOT_FOUND)
	{
		cout << "Theatre with ID : " << theatreId << " not found" << endl;
	}
	if (status == Enums::ProcessStatus::ALREADY_EXISTS)
	{
		cout << "Screen with name " << name << " already exists." << endl;
	}
	return Enums::ProcessStatus::FAILED;
}

/*
* Function Name : updateScreenName
* Description   : Updates the name of a screen.
* Parameters    :
*                  theatre - Theatre containing the screen
*                  screenId - ID of the screen
*                  name     - New screen name
* Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus UserInterface::updateScreenName(const string& theatreId, const string& screenId, const string& name)
{
	Enums::ProcessStatus status = m_controller->updateScreenName(theatreId, screenId, name);
	if (status == Enums::ProcessStatus::SUCCESS)
	{
		cout << "Screen with Name : " << name << " Updated Successfully." << endl;
		return Enums::ProcessStatus::SUCCESS;
	}
	if (status == Enums::ProcessStatus::FAILED)
	{
		cout << "Failed! Could not find theatre with Id : " << theatreId << endl;
	}
	if (status == Enums::ProcessStatus::NOT_FOUND)
	{
		cout << "Screen with ID : " << screenId << " not found in Theatre " << theatreId << endl;
	}
	if (status == Enums::ProcessStatus::ALREADY_EXISTS)
	{
		cout << "Screen with name " << name << " already exists in Theatre " << theatreId << endl;
	}
	return Enums::ProcessStatus::FAILED;
}

/*
* Function Name : deactivateScreen
* Description   : Deactivates a screen.
* Parameters    :
*                  theatre - Theatre containing the screen
*                  screenId - ID of the screen
* Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus UserInterface::deactivateScreen(const string& theatreId, const string& screenId)
{
	Enums::ProcessStatus status = m_controller->deactivateScreen(theatreId, screenId);
	if (status == Enums::ProcessStatus::SUCCESS)
	{
		cout << "Screen with Id : " << screenId << " deactivated Successfully." << endl;
		return Enums::ProcessStatus::SUCCESS;
	}
	if (status == Enums::ProcessStatus::FAILED)
	{
		cout << "Failed! Could not deactivate screen with Id " << screenId << endl;
	}
	if (status == Enums::ProcessStatus::NOT_FOUND)
	{
		cout << "Screen with ID : " << screenId << " not found." << endl;
	}
	if (status == Enums::ProcessStatus::ALREADY_EXISTS)
	{
		cout << "Screen with ID " << screenId << " already been deactivated." << endl;
	}
	return Enums::ProcessStatus::FAILED;
}

/*
* Function Name : reactivateScreen
* Description   : Reactivates an inactive screen.
* Parameters    :
*                  theatre - Theatre containing the screen
*                  screenId - ID of the screen
* Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus UserInterface::reactivateScreen(const string& theatreId, const string& screenId)
{
	Enums::ProcessStatus status = m_controller->reactivateScreen(theatreId, screenId);
	if (status == Enums::ProcessStatus::SUCCESS)
	{
		cout << "Screen with Id : " << screenId << " reactivated Successfully." << endl;
		return Enums::ProcessStatus::SUCCESS;
	}
	if (status == Enums::ProcessStatus::FAILED)
	{
		cout << "Failed! Could not reactivate screen with Id " << screenId << endl;
	}
	if (status == Enums::ProcessStatus::NOT_FOUND)
	{
		cout << "Screen with ID : " << screenId << " not found." << endl;
	}
	if (status == Enums::ProcessStatus::ALREADY_EXISTS)
	{
		cout << "Screen with ID " << screenId << " is active." << endl;
	}
	return Enums::ProcessStatus::FAILED;
}

/*
* Function Name : viewTheatreScreens
* Description   : Displays all screens available in the theatre.
* Parameters    :
*                  theatre - Theatre whose screens are viewed
* Return Type   : void
*/
void UserInterface::viewTheatreScreens(const string& theatreId)
{
	const vector<const Screen*> screens = m_controller->viewTheatreScreens(theatreId);
	if (screens.empty())
	{
		cout << "No screens available." << endl;
		return;
	}
	cout << endl;
	cout << left
		<< setw(15) << "Screen ID"
		<< setw(25) << "Screen Name";
	if (m_controller->getAuthenticatedUserType() == Enums::UserType::ADMIN || m_controller->getAuthenticatedUserType() == Enums::UserType::THEATRE_OWNER)
	{
		cout << setw(25) << "Status";
	}
	cout << endl;
	cout << string(65, '-') << endl;
	for (vector<const Screen*>::const_iterator iterator = screens.begin(); iterator != screens.end(); ++iterator)
	{
		cout << left
			<< setw(15) << (*iterator)->getScreenId()
			<< setw(25) << (*iterator)->getName();
		if (m_controller->getAuthenticatedUserType() == Enums::UserType::ADMIN || m_controller->getAuthenticatedUserType() == Enums::UserType::THEATRE_OWNER)
		{
			cout << setw(25) << Enums::getScreenStatusString((*iterator)->getScreenStatus());
		}
		cout << endl;
	}
	util::pressEnterToContinue();
}

/*
 * Function: displayMovieDetails
 * Description: Displays detailed information about a list of movies in a
 *              formatted tabular view. Outputs movie attributes such as
 *              ID, title, language, genre, duration, and status.
 * Parameters:
 *    movies - A vector of Movie pointers representing the movies
 *             whose details are to be displayed.
 * Returns: None
 */
void UserInterface::displayMovieDetails(const vector<const Movie*>& movies)
{
	cout << string(92, '-') << endl;
	cout << left
		<< setw(14) << "ID"
		<< setw(22) << "Title"
		<< setw(14) << "Language"
		<< setw(18) << "Genre"
		<< setw(10) << "Duration"
		<< setw(14) << "Status"
		<< endl;
	cout << string(92, '-') << endl;
	for (vector<const Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		if (*iterator)
		{
			cout << left
				<< setw(14) << (*iterator)->getMovieId()
				<< setw(22) << (*iterator)->getTitle()
				<< setw(14) << (*iterator)->getLanguage()
				<< setw(18) << (*iterator)->getGenre()
				<< setw(10) << (*iterator)->getDuration()
				<< setw(14) << Enums::getMovieStatusString((*iterator)->getStatus())
				<< endl;
		}
	}
	util::pressEnterToContinue();
}

/*
 * Function: searchTheatre
 * Description: Allows the user to search for theatres by name. Prompts the user
 *              to enter a theatre name, retrieves matching theatres from the
 *              Controller, and displays results based on the authenticated
 *              user's role. Admin users see detailed theatre information, while
 *              regular users see a simplified view.
 * Parameters: None
 * Returns: None
 */
void UserInterface::searchTheatre()
{
	string theatreName;
	util::readValueWithRetry(theatreName, "Enter theatre name: ");
	const vector<const Theatre*> theatres = m_controller->searchTheatreByName(theatreName);
	if (theatres.empty())
	{
		cout << "\nNo theatres found" << endl;
		util::pressEnterToContinue();
		return;
	}
	const User* authenticatedUser = m_controller->getAuthenticatedUser();
	if (authenticatedUser->getUserType() == Enums::UserType::ADMIN)
	{
		displayTheatresForAdmin(theatres);
	}
	else
	{
		displayTheatresForUsers(theatres);
	}
}

/*
 * Function: displayTheatresForAdmin
 * Description: Displays detailed information about a list of theatres in a
 *              formatted tabular view specifically for administrators. Outputs
 *              theatre attributes such as ID, name, city, address, and contact
 *              phone number in a structured layout for easy review.
 * Parameters:
 *    theatres - A vector of Theatre pointers representing the theatres
 *               whose details are to be displayed for the admin user.
 * Returns: None
 */
void UserInterface::displayTheatresForAdmin(const vector<const Theatre*>& theatres)
{
	cout << string(98, '-') << endl;
	cout << left
		<< setw(16) << "ID"
		<< setw(22) << "Name"
		<< setw(16) << "City"
		<< setw(28) << "Address"
		<< setw(16) << "Contact"
		<< endl;
	cout << string(98, '-') << endl;
	for (vector<const Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
	{
		cout << left
			<< setw(16) << (*iterator)->getTheatreId()
			<< setw(22) << (*iterator)->getName()
			<< setw(16) << (*iterator)->getCity()
			<< setw(28) << (*iterator)->getAddress()
			<< setw(16) << (*iterator)->getTheatrePhoneNumber()
			<< endl;
	}
	util::pressEnterToContinue();
}

/*
 * Function: displayTheatresForUsers
 * Description: Displays a simplified view of theatre information tailored for
 *              regular users. Outputs theatre attributes such as name, city,
 *              address, and contact phone number in a formatted tabular layout.
 * Parameters:
 *    theatres - A vector of Theatre pointers representing the theatres
 *               whose details are to be displayed for user view.
 * Returns: None
 */
void UserInterface::displayTheatresForUsers(const vector<const Theatre*>& theatres)
{
	cout << string(82, '-') << endl;
	cout << left
		<< setw(22) << "Name"
		<< setw(16) << "City"
		<< setw(28) << "Address"
		<< setw(16) << "Contact"
		<< endl;
	cout << string(82, '-') << endl;
	for (vector<const Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
	{
		cout << left
			<< setw(22) << (*iterator)->getName()
			<< setw(16) << (*iterator)->getCity()
			<< setw(28) << (*iterator)->getAddress()
			<< setw(16) << (*iterator)->getTheatrePhoneNumber()
			<< endl;
	}
	util::pressEnterToContinue();
}

/*
 * Function: UserInterface::listAllTheatres
 * Description: Provides an interactive menu for users to view theatres based on
 *              their status (Active or Inactive). Prompts the user for a choice,
 *              retrieves all theatres from the Controller, and displays them
 *              according to the selected status. Handles invalid input gracefully.
 * Parameters: None
 * Returns: None
 */
void UserInterface::listAllTheatres()
{
	unsigned short choice;
	cout << "\n1. Active Theatres";
	cout << "\n2. Inactive Theatres";
	util::readValueWithRetry(choice, "\n Enter choice: ");
	const vector<const Theatre*>theatres = m_controller->getAllTheatres();
	if (choice == 1)
	{
		bool isActiveTheatreFound = false;
		for (vector<const Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
		{
			if ((*iterator)->getStatus() == Enums::TheatreStatus::ACTIVE)
			{
				isActiveTheatreFound = true;
				break;
			}
		}
		if (isActiveTheatreFound)
		{
			displayTheatres(theatres, Enums::TheatreStatus::ACTIVE);
		}
		else
		{
			cout << "\nNo active theatres found!";
			util::pressEnterToContinue();
		}
	}
	else if (choice == 2)
	{
		bool isInactiveTheatreFound = false;
		for (vector<const Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
		{
			if ((*iterator)->getStatus() == Enums::TheatreStatus::INACTIVE)
			{
				isInactiveTheatreFound = true;
				break;
			}
		}
		if (isInactiveTheatreFound)
		{
			displayTheatres(theatres, Enums::TheatreStatus::INACTIVE);
		}
		else
		{
			cout << "\nNo inactive theatres found!";
			util::pressEnterToContinue();
		}
	}
	else
	{
		cout << "\nInvalid Choice!";
		util::pressEnterToContinue();
	}
}

/*
 * Function: UserInterface::listAllOwnerTheatres
 * Description: Provides an interactive menu for owners to view theatres based on
 *              their status (Active or Inactive). Prompts the user for a choice,
 *              retrieves all theatres from the Controller, and displays them
 *              according to the selected status. Handles invalid input gracefully.
 * Parameters: None
 * Returns: None
 */
void UserInterface::listAllOwnerTheatres()
{
	unsigned short choice;
	cout << "\n1. Active Theatres";
	cout << "\n2. Inactive Theatres";
	util::readValueWithRetry(choice, "\n Enter choice: ");
	if (choice == 1)
	{
		bool isActiveTheatreFound = false;
		const vector<const Theatre*> theatres = m_controller->getCurrentOwnerTheatres();
		for (vector<const Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
		{
			if ((*iterator)->getStatus() == Enums::TheatreStatus::ACTIVE)
			{
				isActiveTheatreFound = true;
				break;
			}
		}
		if (isActiveTheatreFound)
		{
			displayTheatres(theatres, Enums::TheatreStatus::ACTIVE);
		}
		else
		{
			cout << "\nNo active theatres found!" << endl;
			util::pressEnterToContinue();
		}
	}
	else if (choice == 2)
	{
		bool isInactiveTheatreFound = false;
		const vector<const Theatre*> theatres = m_controller->getCurrentOwnerInactiveTheatres();
		for (vector<const Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
		{
			if ((*iterator)->getStatus() == Enums::TheatreStatus::INACTIVE)
			{
				isInactiveTheatreFound = true;
				break;
			}
		}
		if (isInactiveTheatreFound)
		{
			displayTheatres(theatres, Enums::TheatreStatus::INACTIVE);
		}
		else
		{
			cout << "\nNo inactive theatres found!" << endl;
			util::pressEnterToContinue();
		}
	}
	else
	{
		cout << "\nInvalid Choice!" << endl;
		util::pressEnterToContinue();
	}
}

/*
 * Function: displayTheatres
 * Description: Displays a formatted tabular view of theatres filtered by their
 *              status (Active or Inactive). Outputs theatre attributes such as
 *              ID, name, city, address, contact phone number, and status string
 *              for each theatre that matches the given status.
 * Parameters:
 *    theatres - A vector of Theatre pointers representing the theatres to be displayed.
 *    status   - The TheatreStatus enum value used to filter which theatres are shown.
 * Returns: None
 */
void UserInterface::displayTheatres(const vector<const Theatre*>& theatres, Enums::TheatreStatus status)
{
	cout << string(112, '-') << endl;
	cout << left
		<< setw(16) << "ID"
		<< setw(22) << "Name"
		<< setw(16) << "City"
		<< setw(28) << "Address"
		<< setw(16) << "Contact"
		<< setw(14) << "Status"
		<< endl;
	cout << string(112, '-') << endl;
	for (vector<const Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
	{
		if ((*iterator)->getStatus() == status)
		{
			cout << left
				<< setw(16) << (*iterator)->getTheatreId()
				<< setw(22) << (*iterator)->getName()
				<< setw(16) << (*iterator)->getCity()
				<< setw(28) << (*iterator)->getAddress()
				<< setw(16) << (*iterator)->getTheatrePhoneNumber()
				<< setw(14) << Enums::getTheatreStatusString((*iterator)->getStatus())
				<< endl;
		}
	}
	util::pressEnterToContinue();
}

/*
 * Function: displayMoviesInTheatre
 * Description: Displays movies available in the selected theatre.
 * Parameters:
 *    None
 * Returns:
 *    void
 */
void UserInterface::displayMoviesInTheatre()
{
	string theatreId;
	bool isTheatreIdValid = false;
	const vector<const Theatre*> theatres = m_controller->getCurrentOwnerTheatres();
	displayTheatreDetails(theatres);
	util::readValueWithRetry(theatreId, "Enter theatre id of theatre to select: ");
	const vector<string> theatreIds = m_controller->getCurrentOwnerTheatreIds();
	for (vector<string>::const_iterator iterator = theatreIds.begin(); iterator != theatreIds.end(); ++iterator)
	{
		if (theatreId == *iterator)
		{
			isTheatreIdValid = true;
			break;
		}
	}
	if (!isTheatreIdValid)
	{
		cout << "Invalid Theatre id!" << endl;
		util::pressEnterToContinue();
		return;
	}
	const vector<const Movie*> movies = m_controller->getMoviesFromTheatre(theatreId);
	if (movies.empty())
	{
		cout << "\nNo Movies in theatre\n";
		return;
	}
	displayMovieDetails(movies);
}

/*
 * Function: displayMoviesInTheatre
 * Description: Displays movies available in the selected theatre.
 * Parameters:
 *    theatreId (string&) - Reference to store selected theatre ID
 * Returns:
 *    void
 */
Enums::ProcessStatus UserInterface::displayMoviesInTheatre(string& theatreId)
{
	const vector<const Theatre*> theatres = m_controller->getCurrentOwnerTheatres();
	if (theatres.empty())
	{
		return Enums::ProcessStatus::FAILED;
	}
	const vector<string> theatreIds = m_controller->getCurrentOwnerTheatreIds();
	displayTheatreDetails(theatres);
	bool isTheatreIdValid = false;
	while (!isTheatreIdValid)
	{
		util::readValueWithRetry(theatreId, "Enter theatre id of theatre to select: ");
		for (vector<string>::const_iterator iterator = theatreIds.begin(); iterator != theatreIds.end(); ++iterator)
		{
			if (theatreId == *iterator)
			{
				isTheatreIdValid = true;
				break;
			}
		}
		if (!isTheatreIdValid)
		{
			cout << "Invalid Theatre id! Please try again." << endl;
		}
	}
	const vector<const Movie*> movies = m_controller->getMoviesFromTheatre(theatreId);
	if (movies.empty())
	{
		return Enums::ProcessStatus::FAILED;
	}
	displayMovieDetails(movies);
	return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: validateMovieId
 * Description: Validates whether a given movie ID exists within the list of
 *              available movie IDs. Iterates through the provided IDs and
 *              checks for a match with the input movie ID.
 * Parameters:
 *    movieId  - The movie ID entered by the user to be validated.
 *    movieIds - A vector of strings containing valid movie IDs.
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the movie ID is found in the list.
 *    Enums::ProcessStatus::FAILED if the movie ID does not exist.
 */
Enums::ProcessStatus UserInterface::validateMovieId(const string& movieId, const vector<string>& movieIds)
{
	for (vector<string>::const_iterator iterator = movieIds.begin(); iterator != movieIds.end(); ++iterator)
	{
		if (*iterator == movieId)
		{
			return Enums::ProcessStatus::SUCCESS;
		}
	}
	return Enums::ProcessStatus::FAILED;
}

/*
 * Function: getMovieIds
 * Description: Extracts and returns the list of movie IDs from a given vector
 *              of Movie pointers. Iterates through the provided movies and
 *              collects their unique identifiers into a string vector.
 * Parameters:
 *    movies - A vector of Movie pointers representing the movies from which
 *             IDs are to be extracted.
 * Returns:
 *    A vector of strings containing the IDs of the provided movies.
 */
const vector<string> UserInterface::getMovieIds(const vector<const Movie*>& movies)
{
	vector<string> movieIds;
	for (vector<const Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		movieIds.push_back((*iterator)->getMovieId());
	}
	return movieIds;
}

/*
 * Function: validateTheatreId
 * Description: Validates whether a given theatre ID exists within the list of
 *              available theatre IDs. Iterates through the provided IDs and
 *              checks for a match with the input theatre ID.
 * Parameters:
 *    theatreId  - The theatre ID entered by the user to be validated.
 *    theatreIds - A vector of strings containing valid theatre IDs.
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the theatre ID is found in the list.
 *    Enums::ProcessStatus::FAILED if the theatre ID does not exist.
 */
Enums::ProcessStatus UserInterface::validateTheatreId(const string& theatreId, const vector<string>& theatreIds)
{
	for (vector<string>::const_iterator iterator = theatreIds.begin(); iterator != theatreIds.end(); ++iterator)
	{
		if (*iterator == theatreId)
		{
			return Enums::ProcessStatus::SUCCESS;
		}
	}
	return Enums::ProcessStatus::FAILED;
}

/*
 * Function: getTheatreIds
 * Description: Extracts and returns the list of theatre IDs from a given vector
 *              of Theatre pointers. Iterates through the provided theatres and
 *              collects their unique identifiers into a string vector.
 * Parameters:
 *    theatres - A vector of Theatre pointers representing the theatres from
 *               which IDs are to be extracted.
 * Returns:
 *    A vector of strings containing the IDs of the provided theatres.
 */
const vector<string> UserInterface::getTheatreIds(const vector<const Theatre*>& theatres)
{
	vector<string> theatreIds;
	for (vector<const Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
	{
		theatreIds.push_back((*iterator)->getTheatreId());
	}
	return theatreIds;
}

/*
 * Function: displayOwnerTheatres
 * Description: Displays a simplified tabular view of theatres owned by the
 *              currently authenticated user. Outputs theatre attributes such
 *              as ID, name, and city for each owned theatre.
 * Parameters:
 *    theatres - A vector of Theatre pointers representing the theatres owned
 *               by the authenticated user.
 * Returns: None
 */
void UserInterface::displayOwnerTheatres(const vector<const Theatre*>& theatres)
{
	cout << string(54, '-') << endl;
	cout << left
		<< setw(16) << "ID"
		<< setw(22) << "Name"
		<< setw(16) << "City"
		<< endl;
	cout << string(54, '-') << endl;
	for (vector<const Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
	{
		cout << left
			<< setw(16) << (*iterator)->getTheatreId()
			<< setw(22) << (*iterator)->getName()
			<< setw(16) << (*iterator)->getCity()
			<< endl;
	}
	util::pressEnterToContinue();
}

/*
 * Function: displayMovie
 * Description: Displays a formatted list of movies with their details (ID, Title, Language, Genre, Duration).
 * Parameters:
 *    movies - Vector of constant Movie pointers to display
 * Returns:
 *    None
 */
void UserInterface::displayMovie(const vector<const Movie*>& movies)
{
	cout << string(78, '-') << endl;
	cout << left
		<< setw(14) << "ID"
		<< setw(22) << "Title"
		<< setw(14) << "Language"
		<< setw(18) << "Genre"
		<< setw(10) << "Duration"
		<< endl;
	cout << string(78, '-') << endl;
	for (vector<const Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		if (*iterator)
		{
			cout << left
				<< setw(14) << (*iterator)->getMovieId()
				<< setw(22) << (*iterator)->getTitle()
				<< setw(14) << (*iterator)->getLanguage()
				<< setw(18) << (*iterator)->getGenre()
				<< setw(10) << (*iterator)->getDuration()
				<< endl;
		}
	}
	util::pressEnterToContinue();
}

/*
 * Function: addMovieToTheatre
 * Description: Allows a theatre owner to add a movie to one of their theatres.
 *              Prompts the user to select a theatre and a movie from available
 *              lists, validates the entered IDs, and delegates the addition
 *              process to the Controller. Provides feedback on success or
 *              failure (e.g., movie already exists in theatre).
 * Parameters: None
 * Returns: None
 */
void UserInterface::addMovieToTheatre()
{
	string theatreId;
	string movieId;
	const vector<const Theatre*> theatres = m_controller->getCurrentOwnerTheatres();
	if (theatres.empty())
	{
		cout << "\nNo theatres found!";
		util::pressEnterToContinue();
		return;
	}
	displayOwnerTheatres(theatres);
	const vector<string> theatreIds = getTheatreIds(theatres);
	util::readValueWithRetry(theatreId, "\nEnter Theatre ID: ");
	if (validateTheatreId(theatreId, theatreIds) == Enums::ProcessStatus::FAILED)
	{
		cout << "\nInvalid Theatre ID!";
		util::pressEnterToContinue();
		return;
	}
	const vector<const Movie*> movies = m_controller->getAllActiveMovies();
	if (movies.empty())
	{
		cout << "\nNo active movies found!";
		util::pressEnterToContinue();
		return;
	}
	displayMovie(movies);
	const vector<string> movieIds = getMovieIds(movies);
	util::readValueWithRetry(movieId, "\nEnter Movie ID: ");
	if (validateMovieId(movieId, movieIds) == Enums::ProcessStatus::FAILED)
	{
		cout << "\nInvalid Movie ID!";
		util::pressEnterToContinue();
		return;
	}
	if (m_controller->addMovieToTheatre(theatreId, movieId) == Enums::ProcessStatus::SUCCESS)
	{
		cout << "\nMovie successfully added to theatre!\n";
		util::pressEnterToContinue();
	}
	else
	{
		cout << "\nMovie already exists in theatre!";
		util::pressEnterToContinue();
	}
}

/*
 * Function: getScreenId
 * Description: Displays available screens and validates the selected screen ID.
 * Parameters:
 *    screens (const std::vector<const Screen*>&) - List of available screens
 *    screenId (std::string&) - Reference to store selected screen ID
 * Returns:
 *    bool - True if a valid screen ID is selected, false otherwise
 */
bool UserInterface::getScreenId(const vector<const Screen*>& screens, string& screenId)
{
	vector<string> screenIds;
	cout << "\nAvaiable Screens\n-----------------------------" << endl;
	for (vector<const Screen*>::const_iterator iterator = screens.begin(); iterator != screens.end(); ++iterator)
	{
		cout << (*iterator)->getScreenId() << "   " << (*iterator)->getName() << endl;
		screenIds.push_back((*iterator)->getScreenId());
	}
	util::readValueWithRetry(screenId, "Enter screen id of screen to add to: ");
	for (vector<string>::iterator iterator = screenIds.begin(); iterator != screenIds.end(); ++iterator)
	{
		if ((*iterator) == screenId)
		{
			return true;
		}
	}
	return false;
}

/*
 * Function: handleInputTheatreDetails
 * Description: Validates the entered theatre details by checking whether
 *              the theatre already exists in the system.
 * Parameters:
 *    name (const std::string&) - Name of the theatre
 *    city (const std::string&) - City where the theatre is located
 *    address (const std::string&) - Address of the theatre
 *    phoneNumber (const std::string&) - Contact phone number of the theatre
 *    email (const std::string&) - Contact email address of the theatre
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the theatre details are unique,
 *                           FAILED otherwise
 */
Enums::ProcessStatus UserInterface::handleInputTheatreDetails(const string& name, const string& city, const string& address, const string& phoneNumber, const string& email)
{
	return m_controller->isTheatreUnique(name, city, address, phoneNumber, email);
}

/*
 * Function: addTheatre
 * Description: Collects theatre details from the user, validates the input,
 *              ensures unique phone number and email, and sends the theatre
 *              registration request to the controller for approval.
 * Parameters:
 *    None
 * Returns:
 *    void
 */
void UserInterface::addTheatre()
{
	string name, city, address, phoneNumber, email;
	util::readValueWithRetry(name, "\nEnter the name is Theatre   : ");
	util::readValueWithRetry(city, "\nEnter the city              : ");
	util::readValueWithRetry(address, "\nEnter the address           : ");
	util::readValueWithRetry(phoneNumber, "\nEnter the phone number      : ");
	util::isPhoneNumberValid(phoneNumber);
	getUniqueTheatrePhoneNumber(phoneNumber);
	util::readValueWithRetry(email, "\nEnter the email             : ");
	util::isEmailValid(email);
	getUniqueTheatreEmail(email);
	if (handleInputTheatreDetails(name, city, address, phoneNumber, email) == Enums::ProcessStatus::SUCCESS)
	{
		if (Enums::ProcessStatus::SUCCESS == m_controller->addTheatre(name, city, address, phoneNumber, email))
		{
			cout << "\nTheatre added successfully.\nYour request has been sent to the admin for approval.\n";
		}
		else
		{
			cout << "Something went wrong!.";
		}
		util::pressEnterToContinue();
	}
	else
	{
		cout << "\nTheatre already exist!.";
		util::pressEnterToContinue();
		return;
	}
}

/*
 * Function: UserInterface::getUniqueTheatrePhoneNumber
 * Description: Ensures that the entered theatre phone number is unique
 *              by repeatedly prompting the user until a unique number is entered.
 * Parameters:
 *    phoneNumber (std::string&) - Reference to the theatre phone number
 * Returns:
 *    void
 */
void UserInterface::getUniqueTheatrePhoneNumber(string& phoneNumber)
{
	bool isPhoneNumberUnique = (m_controller->isTheatrePhoneNumberUnique(phoneNumber) == Enums::ProcessStatus::SUCCESS) ? true : false;
	{
		while (!isPhoneNumberUnique)
		{
			util::readValueWithRetry(phoneNumber, "Phone number already exists!. Please enter again: ");
			util::isPhoneNumberValid(phoneNumber);
			if (m_controller->isTheatrePhoneNumberUnique(phoneNumber) == Enums::ProcessStatus::SUCCESS)
			{
				isPhoneNumberUnique = true;
			}
		}
	}
}

/*
 * Function: getUniqueTheatreEmail
 * Description: Ensures that the entered theatre email address is unique
 *              by repeatedly prompting the user until a unique email is entered.
 * Parameters:
 *    email (std::string&) - Reference to the theatre email address
 * Returns:
 *    void
 */
void UserInterface::getUniqueTheatreEmail(string& email)
{
	bool isEmailUnique = (m_controller->isTheatreEmailUnique(email) == Enums::ProcessStatus::SUCCESS) ? true : false;
	while (!isEmailUnique)
	{
		util::readValueWithRetry(email, "Email already exists!. Please enter again: ");
		util::isEmailValid(email);
		if (m_controller->isEmailUnique(email) == Enums::ProcessStatus::SUCCESS)
		{
			isEmailUnique = true;
		}
	}
}

/*
 * Function: isValidTheatreID
 * Description: Checks whether the given theatre ID exists in the provided
 *              collection of theatres.
 * Parameters:
 *    theatreId - Unique identifier of the theatre to validate.
 *    theatres  - Vector containing theatre objects to search.
 * Returns:
 *    ProcessStatus::SUCCESS if the theatre ID exists.
 *    ProcessStatus::FAILED if the theatre ID is not found.
 */
Enums::ProcessStatus UserInterface::isValidTheatreID(const string& theatreId, const vector<const Theatre*>& theatres)
{
	for (vector<const Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
	{
		if ((*iterator)->getTheatreId() == theatreId)
		{
			return Enums::ProcessStatus::SUCCESS;
		}
	}
	return Enums::ProcessStatus::FAILED;
}

/*
 * Function: displayEditTheatreMenu
 * Description: Displays the menu options available for editing theatre
 *              details such as name, city, address, phone number, and email.
 * Parameters: None
 * Returns: None
 */
void UserInterface::displayEditTheatreMenu()
{
	cout << "\n\n1.Name";
	cout << "\n2.City";
	cout << "\n3.Address";
	cout << "\n4.Phone Number";
	cout << "\n5.Email";
	cout << "\n0.Exit";
	cout << "\n-----------------------" << endl;
}

/*
 * Function: updateTheatre
 * Description: Allows the current theatre owner to update theatre details.
 *              Displays available theatres, validates the selected theatre ID,
 *              and provides options to edit theatre information.
 * Parameters: None
 * Returns: None
 */
void UserInterface::updateTheatre()
{
	string theatreId, name, city, address, phoneNumber, email;
	unsigned short choice = 1;
	const vector<const Theatre*> theatres = m_controller->getCurrentOwnerTheatres();
	if (!theatres.empty())
	{
		displayTheatreDetails(theatres);
		util::readValueWithRetry(theatreId, "\nEnter the theatre Id, which you want to edit: ");
		if (isValidTheatreID(theatreId, theatres) == Enums::ProcessStatus::SUCCESS)
		{
			const Theatre* theatre = getCurrentTheatreById(theatreId, theatres);
			while (choice != 0)
			{
				displayEditTheatreMenu();
				util::readValueWithRetry(choice, "\nEnter which details want to edit: ");
				if (choice == 1)
				{
					util::readValueWithRetry(name, "\nEnter the new name: ");
					changeTheatreName(theatreId, name, theatre);
				}
				else if (choice == 2)
				{
					util::readValueWithRetry(city, "\nEnter the new city: ");
					changeTheatreCity(theatreId, city, theatre);
				}
				else if (choice == 3)
				{
					util::readValueWithRetry(address, "\nEnter the new address: ");
					changeTheatreAddress(theatreId, address, theatre);
				}
				else if (choice == 4)
				{
					util::readValueWithRetry(phoneNumber, "\nEnter the new phone number: ");
					util::isPhoneNumberValid(phoneNumber);
					getUniqueTheatrePhoneNumber(phoneNumber);
					changeTheatrePhoneNumber(theatreId, phoneNumber, theatre);
				}
				else if (choice == 5)
				{
					util::readValueWithRetry(email, "\nEnter the new email: ");
					getUniqueTheatreEmail(email);
					changeTheatreEmail(theatreId, email, theatre);
				}
				else
				{
					cout << "\nEnter a valid choice!.";
				}
			}
		}
		else
		{
			cout << "\nEnter the valid theatre id";
			util::pressEnterToContinue();
		}
	}
	else
	{
		cout << "No theatres found has added" << endl;
		util::pressEnterToContinue();
	}
}

/*
 * Function: changeTheatreName
 * Description: Updates the name of the selected theatre after validating
 *              the updated theatre details.
 * Parameters:
 *    theatreId - Unique identifier of the theatre.
 *    name      - New theatre name.
 *    theatre   - Reference to the current theatre object.
 * Returns: None
 */
void UserInterface::changeTheatreName(const string& theatreId, const string& name, const Theatre*& theatre)
{
	if (handleInputTheatreDetails(name, theatre->getCity(), theatre->getAddress(), theatre->getTheatrePhoneNumber(), theatre->getTheatreEmail()) == Enums::ProcessStatus::SUCCESS)
	{
		if (Enums::ProcessStatus::SUCCESS == m_controller->setTheatreNameById(theatreId, name))
		{
			cout << "Name has been updated!.";
		}
		else
		{
			cout << "\nName can't updated!.";
		}
	}
	else
	{
		cout << "\nThis Theatre already exist!.\n";
		return;
	}
}

/*
 * Function: changeTheatreCity
 * Description: Updates the city of the selected theatre after validating
 *              the updated theatre details.
 * Parameters:
 *    theatreId - Unique identifier of the theatre.
 *    city      - New city name.
 *    theatre   - Reference to the current theatre object.
 * Returns: None
 */
void UserInterface::changeTheatreCity(const string& theatreId, const string& city, const Theatre*& theatre)
{
	if (handleInputTheatreDetails(theatre->getName(), city, theatre->getAddress(), theatre->getTheatrePhoneNumber(), theatre->getTheatreEmail()) == Enums::ProcessStatus::SUCCESS)
	{
		if (Enums::ProcessStatus::SUCCESS == m_controller->setTheatreCityById(theatreId, city))
		{
			cout << "\nCity has been updated!.";
		}
		else
		{
			cout << "\nCity can't updated!.";
		}
	}
	else
	{
		cout << "\nThis Theatre already exist!.\n";
		return;
	}
}

/*
 * Function: changeTheatreAddress
 * Description: Updates the address of the selected theatre after validating
 *              the updated theatre details.
 * Parameters:
 *    theatreId - Unique identifier of the theatre.
 *    address   - New address of the theatre.
 *    theatre   - Reference to the current theatre object.
 * Returns: None
 */
void UserInterface::changeTheatreAddress(const string& theatreId, const string& address, const Theatre*& theatre)
{
	if (handleInputTheatreDetails(theatre->getName(), theatre->getCity(), address, theatre->getTheatrePhoneNumber(), theatre->getTheatreEmail()) == Enums::ProcessStatus::SUCCESS)
	{
		if (Enums::ProcessStatus::SUCCESS == m_controller->setTheatreAddressById(theatreId, address))
		{
			cout << "\nAddress has been updated!.";
		}
		else
		{
			cout << "\nAddress can't updated!.";
		}
	}
	else
	{
		cout << "\nThis Theatre already exist!.\n";
		return;
	}
}

/*
 * Function: changeTheatrePhoneNumber
 * Description: Updates the phone number of the selected theatre after
 *              validating the updated theatre details.
 * Parameters:
 *    theatreId   - Unique identifier of the theatre.
 *    phoneNumber - New phone number of the theatre.
 *    theatre     - Reference to the current theatre object.
 * Returns: None
 */
void UserInterface::changeTheatrePhoneNumber(const string& theatreId, const string& phoneNumber, const Theatre*& theatre)
{
	if (handleInputTheatreDetails(theatre->getName(), theatre->getCity(), theatre->getAddress(), phoneNumber, theatre->getTheatreEmail()) == Enums::ProcessStatus::SUCCESS)
	{
		if (Enums::ProcessStatus::SUCCESS == m_controller->setTheatrePhoneNumberById(theatreId, phoneNumber))
		{
			cout << "\nPhone number has been updated!.";
		}
		else
		{
			cout << "\nPhone Number can't updated!.";
		}
	}
	else
	{
		cout << "\nThis Theatre already exist!.\n";
		return;
	}
}

/*
 * Function: changeTheatreEmail
 * Description: Updates the email address of the selected theatre after
 *              validating the updated theatre details.
 * Parameters:
 *    theatreId - Unique identifier of the theatre.
 *    email     - New email address of the theatre.
 *    theatre   - Reference to the current theatre object.
 * Returns: None
 */
void UserInterface::changeTheatreEmail(const string& theatreId, const string& email, const Theatre*& theatre)
{
	if (handleInputTheatreDetails(theatre->getName(), theatre->getCity(), theatre->getAddress(), theatre->getTheatrePhoneNumber(), email) == Enums::ProcessStatus::SUCCESS)
	{
		if (Enums::ProcessStatus::SUCCESS == m_controller->setTheatreEmailById(theatreId, email))
		{
			cout << "\nEmail has been updated!.";
		}
		else
		{
			cout << "\nEmail can't updated!.";
		}
	}
	else
	{
		cout << "\nThis Theatre already exist!.\n";
		return;
	}
}

/*
 * Function: getCurrentTheatreById
 * Description: Retrieves the theatre object that matches the given
 *              theatre ID from the provided theatre collection.
 * Parameters:
 *    theatreId - Unique identifier of the theatre.
 *    theatres  - Vector containing theatre objects.
 * Returns:
 *    Pointer to the matching Theatre object if found.
 */
const Theatre* UserInterface::getCurrentTheatreById(const string& theatreId, const vector<const Theatre*>& theatres)
{
	for (vector<const Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
	{
		if ((*iterator)->getTheatreId() == theatreId)
		{
			return *iterator;
		}
	}
	return nullptr;
}

/*
 * Function: validateTheatreRequest
 * Description: Allows the administrator to validate pending theatre
 *              requests by approving or rejecting a selected theatre.
 *              Displays all pending theatres, validates the selected
 *              theatre ID, and updates the theatre status based on
 *              the administrator's choice.
 * Parameters: None
 * Returns: None
 */
void UserInterface::validateTheatreRequest()
{
	string theatreId;
	unsigned short choice;
	const vector<const Theatre*> pendingTheatres = m_controller->getPendingTheatres();
	if (!pendingTheatres.empty())
	{
		displayTheatreDetails(pendingTheatres);
		util::readValueWithRetry(theatreId, "\nEnter the theatre Id, which you want to validate: ");
		if (isValidTheatreID(theatreId, pendingTheatres) == Enums::ProcessStatus::SUCCESS)
		{
			displayTheatreValidationMenu();
			util::readValue(choice);
			if (choice == 1)
			{
				setTheatreStatusById(theatreId, Enums::TheatreStatus::ACTIVE);
			}
			else if (choice == 2)
			{
				setTheatreStatusById(theatreId, Enums::TheatreStatus::PENDING);
			}
			else if (choice == 0)
			{
				exit(0);
			}
			else
			{
				cout << "\nEnter a valid choice!.";
			}
		}
		else
		{
			cout << "\nEnter the valid theatre id";
		}
	}
	else
	{
		cout << "\nNo pending theatre request!";
	}
	util::pressEnterToContinue();
}

/*
 * Function: displayTheatreValidationMenu
 * Description: Displays the theatre validation menu with options to
 *              approve, reject, or exit the validation process.
 * Parameters: None
 * Returns: None
 */
void UserInterface::displayTheatreValidationMenu()
{
	cout << "\n1. Approve";
	cout << "\n2. Reject";
	cout << "\n0. Exit";
	cout << "\nEnter the choice: ";
}

/*
 * Function: UserInterface::setTheatreStatusById
 * Description: Updates the status of a theatre identified by the given
 *              theatre ID. Delegates the status update operation to
 *              the controller layer.
 * Parameters:
 *    theatreId    - Unique identifier of the theatre.
 *    theatreStatus - New status to be assigned to the theatre.
 * Returns: None
 */
void UserInterface::setTheatreStatusById(const string& theatreId, Enums::TheatreStatus theatreStatus)
{
	if (m_controller->setTheatreStatusById(theatreId, theatreStatus) == Enums::ProcessStatus::SUCCESS)
	{
		cout << "\nTheatre Status has been updated!." << endl;
	}
	else
	{
		cout << "\nTheatre status cannot be updated as it has scheduled shows!.\n";
	}
}

/*
 * Function: deactivateTheatreByOwner
 * Description: Allows the current theatre owner to deactivate one of
 *              their theatres by selecting a valid theatre ID from
 *              the list of owned theatres. Updates the selected
 *              theatre's status to INACTIVE.
 * Parameters: None
 * Returns: None
 */
void UserInterface::deactivateTheatreByOwner()
{
	string theatreId;
	const vector<const Theatre*> theatres = m_controller->getCurrentOwnerTheatres();
	if (!theatres.empty())
	{
		displayTheatreDetails(theatres);
		util::readValueWithRetry(theatreId, "\nEnter the theatre Id, which you want to edit: ");
		if (isValidTheatreID(theatreId, theatres) == Enums::ProcessStatus::SUCCESS)
		{
			setTheatreStatusById(theatreId, Enums::TheatreStatus::INACTIVE);
		}
		else
		{
			cout << "\nEnter the valid theatre id";
		}
	}
	else
	{
		cout << "No theatres found for current owner" << endl;
	}
	util::pressEnterToContinue();
}

/*
 * Function: deactivateTheatreByAdmin
 * Description: Allows the administrator to deactivate a theatre by
 *              selecting a valid theatre ID from the list of all
 *              available theatres. Updates the selected theatre's
 *              status to INACTIVE.
 * Parameters: None
 * Returns: None
 */
void UserInterface::deactivateTheatreByAdmin()
{
	string theatreId;
	const vector<const Theatre*> theatres = m_controller->listAllTheatres();
	if (!theatres.empty())
	{
		displayTheatreDetails(theatres);
		util::readValueWithRetry(theatreId, "\nEnter the theatre Id, which you want to edit: ");
		if (isValidTheatreID(theatreId, theatres) == Enums::ProcessStatus::SUCCESS)
		{
			setTheatreStatusById(theatreId, Enums::TheatreStatus::INACTIVE);
		}
		else
		{
			cout << "\nEnter the valid theatre id\n";
		}
	}
	else
	{
		cout << "No theatres found for current owner" << endl;
	}
	util::pressEnterToContinue();
}

/*
 * Function: reactivateTheatreByOwner
 * Description: Allows the current theatre owner to deactivate one of
 *              their theatres by selecting a valid theatre ID from
 *              the list of owned theatres. Updates the selected
 *              theatre's status to ACTIVE.
 * Parameters: None
 * Returns: None
 */
void UserInterface::reactivateTheatreByOwner()
{
	string theatreId;
	const vector<const Theatre*> theatres = m_controller->getCurrentOwnerInactiveTheatres();
	if (!theatres.empty())
	{
		displayTheatreDetails(theatres);
		util::readValueWithRetry(theatreId, "\nEnter the theatre Id, which you want to edit: ");
		if (isValidTheatreID(theatreId, theatres) == Enums::ProcessStatus::SUCCESS)
		{
			setTheatreStatusById(theatreId, Enums::TheatreStatus::PENDING);
		}
		else
		{
			cout << "\nEnter the valid theatre id\n";
		}
	}
	else
	{
		cout << "No theatres found for current owner" << endl;
	}
	util::pressEnterToContinue();
}

/*
 * Function: reactivateTheatreByAdmin
 * Description: Allows the administrator to reactivate a theatre by
 *              selecting a valid theatre ID from the list of all
 *              available theatres. Updates the selected theatre's
 *              status to ACTIVE.
 * Parameters: None
 * Returns: None
 */
void UserInterface::reactivateTheatreByAdmin()
{
	string theatreId;
	const vector<const Theatre*> theatres = m_controller->listAllTheatres();
	if (!theatres.empty())
	{
		displayTheatreDetails(theatres);
		util::readValueWithRetry(theatreId, "\nEnter the theatre Id, which you want to edit: ");
		if (isValidTheatreID(theatreId, theatres) == Enums::ProcessStatus::SUCCESS)
		{
			setTheatreStatusById(theatreId, Enums::TheatreStatus::ACTIVE);
		}
		else
		{
			cout << "\nEnter the valid theatre id\n";
		}
	}
	else
	{
		cout << "No theatres found for current owner" << endl;
	}
	util::pressEnterToContinue();
}

/*
*Function: displayActiveShows
* Description : Displays all active shows retrieved from Controller in tabular format.
* Returns :
	*void
*/
void UserInterface::displayActiveShows()
{
	const vector<const Show*> shows = m_controller->getActiveShows();
	if (shows.empty())
	{
		cout << "No shows available!" << endl;
		util::pressEnterToContinue();
		return;
	}
	displayShowDetails(shows);
}

/*
 * Function: displayAllShows
 * Description: Displays all shows in tabular format for the authenticated theatre owner.
 * Returns:
 *    void
 */
void UserInterface::displayAllShows()
{
	const vector<const Show*> shows = m_controller->getAllShows();
	if (shows.empty())
	{
		cout << "No shows available!" << endl;
		util::pressEnterToContinue();
		return;
	}
	displayShowDetails(shows);
}

/*
 * Function: displayShowDetails
 * Description: Displays detailed information for a list of shows including theatre, screen, movie, and start time.
 * Parameters:
 *    const std::vector<const Show*> shows - List of shows to display
 * Returns:
 *    void
 */
void UserInterface::displayShowDetails(const vector<const Show*> shows)
{
	if (shows.empty())
	{
		cout << "NO SHOWS TO DISPLAY!" << endl;
		return;
	}
	cout << string(98, '-') << endl;
	cout << left
		<< setw(14) << "ID"
		<< setw(22) << "Theatre Name"
		<< setw(14) << "Screen ID"
		<< setw(26) << "Movie Title"
		<< setw(22) << "Show Date and Time"
		<< endl;
	cout << string(98, '-') << endl;
	for (vector<const Show*>::const_iterator iterator = shows.begin(); iterator != shows.end(); ++iterator)
	{
		const Show* show = *iterator;
		if (!show)
		{
			continue;
		}
		const Screen* screen = show->getScreen();
		const Theatre* theatre = (screen ? screen->getTheatre() : nullptr);
		const Movie* movie = show->getMovie();
		cout << left
			<< setw(14) << show->getShowId()
			<< setw(22) << (theatre ? theatre->getName() : "N/A")
			<< setw(14) << (screen ? screen->getScreenId() : "N/A")
			<< setw(26) << (movie ? movie->getTitle() : "N/A")
			<< setw(22) << displayTimeAndDate(show->getStartTime())
			<< endl;
	}
	util::pressEnterToContinue();
}

/*
 * Function: displayTimeAndDate
 * Description: Formats a time_t value into a human-readable string "YYYY-MM-DD HH:MM:SS".
 * Parameters:
 *    time_t time - The time value to format
 * Returns:
 *    std::string - Formatted date/time string
 */
string UserInterface::displayTimeAndDate(time_t time)
{
	tm local{};
	localtime_s(&local, &time);
	ostringstream outputStream;
	outputStream << (1900 + local.tm_year) << "-"
		<< setw(2) << setfill('0') << (1 + local.tm_mon) << "-"
		<< setw(2) << setfill('0') << local.tm_mday << " "
		<< setw(2) << setfill('0') << local.tm_hour << ":"
		<< setw(2) << setfill('0') << local.tm_min << ":"
		<< setw(2) << setfill('0') << local.tm_sec;
	return outputStream.str();
}

/*
 * Function: cancelShow
 * Description: Handles user interaction to cancel a show by validating the show ID,
 *              checking cancellability, and updating the show status.
 * Parameters: None
 * Returns:
 *    void
 */
void UserInterface::cancelShow()
{
	string showId;
	displayActiveShows();
	util::readValueWithRetry(showId, "Enter the show id of show to cancel: ");
	const vector<string> showIds = m_controller->getActiveShowIds();
	bool isShowIdValid = false;
	for (vector<string>::const_iterator iterator = showIds.begin(); iterator != showIds.end(); ++iterator)
	{
		if (*iterator == showId)
		{
			isShowIdValid = true;
			break;
		}
	}
	if (!isShowIdValid)
	{
		cout << "Show id is not valid!" << endl;
		util::pressEnterToContinue();
		return;
	}
	Enums::ProcessStatus isShowCancellable = m_controller->isShowChangable(showId);
	if (isShowCancellable == Enums::ProcessStatus::FAILED)
	{
		cout << "Show is not cancellable" << endl;
		util::pressEnterToContinue();
		util::clear();
		return;
	}
	Enums::ProcessStatus status = m_controller->setShowStatusById(showId, Enums::ShowStatus::CANCELLED);
	if (status == Enums::ProcessStatus::SUCCESS)
	{
		cout << "Show cancelled successfully" << endl;
		util::pressEnterToContinue();
		util::clear();
	}
	else
	{
		cout << "Failed to cancel show" << endl;
		util::pressEnterToContinue();
		util::clear();
	}
}

/*
 * Function: viewShowStatus
 * Description: Allows the user to view the status of a specific show by ID.
 * Returns:
 *    void
 */
void UserInterface::viewShowStatus()
{
	string showId;
	const vector<const Show*> shows = m_controller->getAllShows();
	if (shows.empty())
	{
		cout << "No shows available" << endl;
		util::pressEnterToContinue();
		util::clear();
		return;
	}
	displayAllShows();
	util::readValueWithRetry(showId, "Enter the show id of show to see status of: ");
	const vector<string> showIds = m_controller->getAllShowIds();
	bool isShowIdValid = false;
	for (vector<string>::const_iterator iterator = showIds.begin(); iterator != showIds.end(); ++iterator)
	{
		if (*iterator == showId)
		{
			isShowIdValid = true;
			break;
		}
	}
	if (!isShowIdValid)
	{
		cout << "Show id is not valid!" << endl;
		util::pressEnterToContinue();
		return;
	}
	Enums::ShowStatus status = m_controller->getShowStatus(showId);
	cout << " Show Status: " << Enums::getShowStatusString(status) << endl;
	util::pressEnterToContinue();
	util::clear();
}

/*
 * Function: updateShow
 * Description: Handles user interaction to update a show’s time after validation.
 * Returns:
 *    void
 */
void UserInterface::updateShow()
{
	string showId;
	displayAllShows();
	util::readValueWithRetry(showId, "Enter the show id of show to update: ");
	const vector<string> showIds = m_controller->getAllShowIds();
	bool isShowIdValid = false;
	for (vector<string>::const_iterator iterator = showIds.begin(); iterator != showIds.end(); ++iterator)
	{
		if (*iterator == showId)
		{
			isShowIdValid = true;
			break;
		}
	}
	if (!isShowIdValid)
	{
		cout << "Show id is not valid!" << endl;
		util::pressEnterToContinue();
		return;
	}
	Enums::ProcessStatus isShowUpdatable = m_controller->isShowChangable(showId);
	if (isShowUpdatable == Enums::ProcessStatus::FAILED)
	{
		cout << "Show cannot be updated because it has completed bookings!" << endl;
		util::pressEnterToContinue();
		util::clear();
	}
	time_t newTimeAndDate;
	Enums::ProcessStatus status = getNewDateAndTime(newTimeAndDate);
	if (status == Enums::ProcessStatus::FAILED)
	{
		cout << "Invalid time!" << endl;
		util::pressEnterToContinue();
		util::clear();
		return;
	}
	if (m_controller->isNewShowTimeConflicting(showId, newTimeAndDate) == Enums::ProcessStatus::FAILED)
	{
		cout << "Cannot update show time as it conflicts with the time of another show!" << endl;
		util::pressEnterToContinue();
		util::clear();
		return;
	}
	if (m_controller->updateShow(newTimeAndDate, showId) == Enums::ProcessStatus::SUCCESS)
	{
		cout << "Show updated successfully!" << endl;
		util::pressEnterToContinue();
	}
	else
	{
		cout << "Failed to update show!" << endl;
		util::pressEnterToContinue();
	}
}

/*
 * Function: getNewDateAndTime
 * Description: Reads and validates new date/time input from user, ensuring it is in the future.
 * Parameters:
 *    time (time_t&) - Reference to store the constructed time
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if valid, FAILED otherwise
 */
Enums::ProcessStatus UserInterface::getNewDateAndTime(time_t& time)
{
	int year, month, day, startTimeHour, startTimeMinute;
	cout << "Enter date (YYYY MM DD): ";
	util::readValueWithRetry(year, "Enter date - Year (YYYY): ");
	util::readValueWithRetry(month, "Enter date - Month (MM): ");
	util::readValueWithRetry(day, "Enter day - Day (DD): ");
	getValidDate(year, month, day);
	util::readValueWithRetry(startTimeHour, "Enter start time - Hour (HH): ");
	util::readValueWithRetry(startTimeMinute, "Enter start time - Minute (Minute): ");
	getValidTime(startTimeHour, startTimeMinute);
	if (!isFutureDateTime(year, month, day, startTimeHour, startTimeMinute))
	{
		cout << "Cannot update a show with a past time/date!" << endl;
		util::pressEnterToContinue();
		util::clear();
		return Enums::ProcessStatus::FAILED;
	}
	time = util::createTime(year, month, day, startTimeHour, startTimeMinute);
	if (time == -1)
	{
		return Enums::ProcessStatus::FAILED;
	}
	return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: listShowsForAMovie
 * Description: Displays all active movies, validates user input, and lists shows for the selected movie.
 * Parameters:
 *    None
 * Returns:
 *    void
 */
void UserInterface::listShowsForAMovie()
{
	vector<const Movie*> movies = m_controller->getAllActiveMovies();
	string movieId;
	displayAllMovies();
	if (validateMovieIdInput(movies, movieId) == Enums::ProcessStatus::FAILED)
	{
		cout << "Invalid movie id!" << endl;
		util::pressEnterToContinue();
		util::clear();
		return;
	}
	const vector<const Show*> shows = m_controller->getShowsForMovie(movieId);
	displayShowDetails(shows);
}

/*
* Function Name : listShowsForAMovie
* Description   : Displays available shows for the selected movie and
*                 allows the user to choose a show for booking.
* Parameters    :
*                  movieId - Reference used to store the selected movie ID
*                  showId  - Reference used to store the selected show ID
* Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus UserInterface::listShowsForAMovie(string& movieId, string& showId)
{
	vector<const Movie*> movies = m_controller->getAllActiveMovies();
	displayAllMovies();
	if (validateMovieIdInput(movies, movieId) == Enums::ProcessStatus::FAILED)
	{
		cout << "Invalid movie id!" << endl;
		util::pressEnterToContinue();
		return Enums::ProcessStatus::FAILED;
	}
	const vector<const Show*> shows = m_controller->getShowsForMovie(movieId);
	if (shows.empty())
	{
		cout << "No shows available for movie!" << endl;
		util::pressEnterToContinue();
		return Enums::ProcessStatus::FAILED;
	}
	displayShowDetails(shows);
	util::readValueWithRetry(showId, "Select the show to book seats for: ");
	bool isShowIdValid = false;
	for (vector<const Show*>::const_iterator iterator = shows.begin(); iterator != shows.end(); ++iterator)
	{
		if ((*iterator) != nullptr && (*iterator)->getShowId() == showId)
		{
			isShowIdValid = true;
			break;
		}
	}
	if (!isShowIdValid)
	{
		cout << "Invalid show id!" << endl;
		util::pressEnterToContinue();
		return Enums::ProcessStatus::FAILED;
	}
	return Enums::ProcessStatus::SUCCESS;
}

/*
* Function Name : viewActiveTicketDetails
* Description   : Displays all active tickets for the authenticated user.
*                 Active tickets are retrieved from the controller and shown
*                 using the viewTicketDetails helper.
* Parameters    : None
* Return Type   : void
*/
void UserInterface::viewActiveTicketDetails()
{
	const vector<const Ticket*> tickets = m_controller->viewTicketDetails();
	if (tickets.empty())
	{
		cout << "No Tickets Available" << endl;
		util::pressEnterToContinue();
		return;
	}
	viewTicketDetails(tickets);
}

/*
* Function Name : viewAllTickets
* Description   : Displays all tickets available in the system by retrieving
*                 them from the controller and showing details.
* Parameters    : None
* Return Type   : void
*/
void UserInterface::viewAllTickets()
{
	const vector<const Ticket*> tickets = m_controller->viewAllTickets();
	if (tickets.empty())
	{
		cout << "No Tickets Available" << endl;
		util::pressEnterToContinue();
		return;
	}
	viewTicketDetails(tickets);
}

/*
* Function Name : viewTicketHistory
* Description   : Displays the complete ticket history for the authenticated user.
*                 Tickets are retrieved from the controller and shown using the helper.
* Parameters    : None
* Return Type   : void
*/
void UserInterface::viewTicketHistory()
{
	const vector<const Ticket*> tickets = m_controller->viewTicketHistory();
	if (tickets.empty())
	{
		cout << "No Tickets Available" << endl;
		return;
	}
	viewTicketDetails(tickets);
}

/*
* Function Name : viewTicketDetails
* Description   : Helper function to display details of tickets including
*                 Ticket ID, Payment ID, Amount, Booking ID, and Ticket Status.
* Parameters    :
*                  tickets - Vector of Ticket pointers to display
* Return Type   : void
*/
void UserInterface::viewTicketDetails(const vector<const Ticket*>& tickets)
{
	Enums::UserType type = m_controller->getAuthenticatedUserType();
	cout << left
		<< setw(12) << "Ticket ID";
	if (type == Enums::UserType::ADMIN)
	{
		cout << setw(15) << "Customer Name";
	}
	cout << setw(12) << "Payment ID"
		<< setw(10) << "Amount"
		<< setw(12) << "Booking ID"
		<< setw(12) << "Status"
		<< endl;
	cout << string(73, '-') << endl;
	for (vector<const Ticket*>::const_iterator iterator = tickets.begin(); iterator != tickets.end(); ++iterator)
	{
		const Ticket* ticket = *iterator;
		cout << left << setw(12) << ticket->getTicketId();
		if (type == Enums::UserType::ADMIN)
		{
			cout << setw(15) << ticket->getCustomer()->getUserName();
		}
		cout << setw(12) << ticket->getPayment()->getPaymentId()
			<< setw(10) << ticket->getPayment()->getAmount()
			<< setw(12) << ticket->getPayment()->getBooking()->getBookingId()
			<< setw(12) << Enums::getTicketStatusString(ticket->getTicketStatus())
			<< endl;
	}
	util::pressEnterToContinue();

}

/*
Function Name : handleCardPayment
Description   : Handles card-based payment input (Credit/Debit).
				Prompts the user for card details with retry until valid input is provided.
Parameters    :
				 type - The payment method (Credit or Debit Card)
Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus UserInterface::handleCardPayment(Enums::PaymentMethod type)
{
	type = (type == Enums::PaymentMethod::CREDIT_CARD) ? Enums::PaymentMethod::CREDIT_CARD : Enums::PaymentMethod::DEBIT_CARD;
	string cardNumber, expiry, cvv;
	util::readValueWithRetry(cardNumber, "Enter Card Number: ");
	util::readValueWithRetry(expiry, "Enter Expiry (MM/YY): ");
	util::readValueWithRetry(cvv, "Enter CVV: ");
	while (!util::validateCard(cardNumber, expiry, cvv))
	{
		cout << "Error: Invalid card details. Please try again.\n";
		util::readValueWithRetry(cardNumber, "Enter Card Number: ");
		util::readValueWithRetry(expiry, "Enter Expiry (MM/YY): ");
		util::readValueWithRetry(cvv, "Enter CVV: ");
	}
	return Enums::ProcessStatus::SUCCESS;
}

/*
Function Name : handleUPIPayment
Description   : Handles UPI-based payment input.
				Prompts the user for UPI ID with retry until valid input is provided.
Parameters    :
				 type - The payment method (UPI)
Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus UserInterface::handleUPIPayment(Enums::PaymentMethod type)
{
	type = Enums::PaymentMethod::UPI;
	string upiId;
	util::readValueWithRetry(upiId, "Enter UPI ID: ");
	while (!util::validateUPI(upiId))
	{
		cout << "Error: Invalid UPI ID. Please try again.\n";
		util::readValueWithRetry(upiId, "Enter UPI ID: ");
	}
	return Enums::ProcessStatus::SUCCESS;
}

/*
Function Name : displayPaymentOptions
Description   : Displays available payment methods to the user
				and reads their choice.
Parameters    : None
Return Type   : int
*/
int UserInterface::displayPaymentOptions()
{
	cout << "Select Payment Method:\n";
	cout << "1. Credit Card\n";
	cout << "2. Debit Card\n";
	cout << "3. UPI\n";
	cout << "--------------------------" << endl;
	unsigned short choice;
	util::readValueWithRetry(choice, "Enter choice: ");
	return choice;
}

/*
* Function Name : viewAllPayments
* Description   : Displays all payments relevant to the authenticated user in a formatted
*                 tabular view. Retrieves payments from the controller and prints details
*                 including Payment ID, Booking ID, Amount, Method, Status, and Timestamp.
* Parameters    : None
* Return Type   : void
*/

void UserInterface::viewAllPayments()
{
	const vector<Payment*> allPayments = m_controller->getAllPayments();
	if (allPayments.empty())
	{
		cout << "No Payment Record!" << endl;
		util::pressEnterToContinue();
		return;
	}
	cout << string(94, '-') << endl;
	cout << left
		<< setw(14) << "Payment ID"
		<< setw(14) << "Booking ID"
		<< setw(12) << "Amount"
		<< setw(18) << "Method"
		<< setw(14) << "Status"
		<< setw(22) << "Time"
		<< endl;
	cout << string(94, '-') << endl;
	for (auto iterator = allPayments.begin(); iterator != allPayments.end(); ++iterator)
	{
		Payment* payment = *iterator;
		cout << left
			<< setw(14) << payment->getPaymentId()
			<< setw(14) << payment->getBooking()->getBookingId()
			<< setw(12) << payment->getAmount()
			<< setw(18) << Enums::getPaymentMethodString(payment->getPaymentMethod())
			<< setw(14) << Enums::getPaymentStatusString(payment->getStatus())
			<< setw(22) << util::serializeTime(payment->getTimeStamp())
			<< endl;
	}
	util::pressEnterToContinue();
}

/*
* Function Name : initiatePayment
* Description   : Allows the user to choose a payment method and initiates
*                 payment for the specified booking amount.
* Parameters    :
*                  bookingId - Unique identifier of the booking
*                  amount    - Amount to be paid
* Return Type   : void
*/
void UserInterface::initiatePayment(const string& bookingId, double amount)
{
	unsigned short choice = displayPaymentOptions();
	Enums::PaymentMethod type = Enums::PaymentMethod::UPI;
	Enums::ProcessStatus valid;

	if (choice == 1)
	{
		type = Enums::PaymentMethod::CREDIT_CARD;
		valid = handleCardPayment(type);
	}
	else if (choice == 2)
	{
		type = Enums::PaymentMethod::DEBIT_CARD;
		valid = handleCardPayment(type);
	}
	else if (choice == 3)
	{
		valid = handleUPIPayment(type);
	}
	else
	{
		cout << "Error: Invalid payment method.\n";
		return;
	}
	cout << "Payment method selected successfully.\n";
	Enums::ProcessStatus result = Enums::ProcessStatus::FAILED;
	result = m_controller->initiatePayment(bookingId, type, amount);
	if (result == Enums::ProcessStatus::SUCCESS)
	{
		cout << "Payment completed and ticket generated.\n";
	}
	else
	{
		cout << "Payment failed. Please book again\n";
	}
	util::pressEnterToContinue();
}

/*
 * Function: viewAllBookings
 * Description: Displays bookings based on authenticated user type (customer or theatre owner).
 * Parameters:
 *    None
 * Returns:
 *    void
 */
void UserInterface::viewAllBookings()
{
	const vector<const Booking*>  bookings = m_controller->getAllBookings();
	Enums::UserType userType = getAuthenticatedUserType();
	switch (userType)
	{
	case Enums::UserType::CUSTOMER:
		displayCustomerBookings(bookings);
		break;
	case Enums::UserType::THEATRE_OWNER:
		displayTheatreBookings(bookings);
	default:
		cout << "No Bookings available" << endl;
		util::pressEnterToContinue();
		break;
	}
}

/*
 * Function: getAuthenticatedUserType
 * Description: Determines the type of the currently authenticated user.
 * Parameters:
 *    None
 * Returns:
 *    Enums::UserType - Type of the authenticated user or USER_NOT_FOUND
 */
Enums::UserType UserInterface::getAuthenticatedUserType()
{
	const User* user = m_controller->getAuthenticatedUser();
	if (user != nullptr)
	{
		return user->getUserType();
	}
	return Enums::UserType::USER_NOT_FOUND;
}

/*
 * Function: displayCustomerBookings
 * Description: Displays customer bookings in tabular format including Movie ID, Movie Name,
 *              Date, Theatre Name, number of seats booked, and booking status.
 * Parameters:
 *    bookings (const std::vector<const Booking*>) - List of customer bookings
 * Returns:
 *    void
 */
void UserInterface::displayCustomerBookings(const vector<const Booking*> bookings)
{
	if (bookings.empty())
	{
		cout << "No Bookings Available" << endl;
		util::pressEnterToContinue();
		util::clear();
		return;
	}
	cout << string(122, '-') << endl;
	cout << left
		<< setw(18) << "Booking ID"
		<< setw(22) << "Movie Name"
		<< setw(22) << "Date"
		<< setw(28) << "No of Bookings"
		<< setw(16) << "Booking Status"
		<< endl;
	cout << string(122, '-') << endl;
	for (vector<const Booking*>::const_iterator iterator = bookings.begin(); iterator != bookings.end(); ++iterator)
	{
		if (*iterator != nullptr)
		{
			const Show* show = (*iterator)->getShow();
			if (show != nullptr)
			{
				const Movie* movie = show->getMovie();
				const Screen* screen = show->getScreen();
				const Theatre* theatre = nullptr;
				if (screen != nullptr)
				{
					theatre = screen->getTheatre();
				}
				if (movie != nullptr && theatre != nullptr)
				{
					cout << left
						<< setw(18) << (*iterator)->getBookingId()
						<< setw(22) << movie->getTitle()
						<< setw(22) << displayTimeAndDate(show->getStartTime())
						<< setw(28) << (*iterator)->getBookedSeats().size()
						<< setw(16) << Enums::getBookingStatusString((*iterator)->getStatus())
						<< endl;
				}
			}
		}
	}
	util::pressEnterToContinue();
}

/*
 * Function: displayTheatreBookings
 * Description: Displays theatre bookings grouped by theatres owned by the authenticated user.
 *              Shows Movie ID, Movie Name, Date, Theatre Name, number of seats booked, and booking status.
 * Parameters:
 *    bookings (const std::vector<const Booking*>) - List of theatre bookings
 * Returns:
 *    void
 */
void UserInterface::displayTheatreBookings(const vector<const Booking*> bookings)
{
	if (bookings.empty())
	{
		cout << "No Bookings Available" << endl;
		util::pressEnterToContinue();
		util::clear();
		return;
	}
	const vector<const Theatre*> theatres = m_controller->getCurrentOwnerTheatres();
	bool anyBookingsDisplayed = false;
	for (vector<const Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
	{
		if ((*iterator) == nullptr)
		{
			continue;
		}
		vector<const Booking*> theatreBookings;
		for (vector<const Booking*>::const_iterator bookingsIterator = bookings.begin(); bookingsIterator != bookings.end(); ++bookingsIterator)
		{
			if (*bookingsIterator == nullptr)
			{
				continue;
			}
			const Show* show = (*bookingsIterator)->getShow();
			if (show == nullptr)
			{
				continue;
			}
			const Screen* screen = show->getScreen();
			if (screen == nullptr)
			{
				continue;
			}
			const Theatre* theatre = screen->getTheatre();
			if (theatre != nullptr && theatre->getTheatreId() == (*iterator)->getTheatreId())
			{
				theatreBookings.push_back(*bookingsIterator);
			}
		}
		if (theatreBookings.empty())
		{
			continue;
		}
		cout << (*iterator)->getName() << " Bookings\n---------------------" << endl;
		cout << string(122, '-') << endl;
		cout << left
			<< setw(18) << "Booking ID"
			<< setw(22) << "Movie Name"
			<< setw(22) << "Date"
			<< setw(28) << "Theater Name"
			<< setw(16) << "No of Bookings"
			<< setw(16) << "Booking Status"
			<< endl;
		cout << string(122, '-') << endl;
		for (vector<const Booking*>::const_iterator bookingsIterator = theatreBookings.begin(); bookingsIterator != theatreBookings.end(); ++bookingsIterator)
		{
			const Show* show = (*bookingsIterator)->getShow();
			const Movie* movie = show->getMovie();
			const Theatre* theatre = show->getScreen()->getTheatre();
			cout << left
				<< setw(18) << (*bookingsIterator)->getBookingId()
				<< setw(22) << movie->getTitle()
				<< setw(22) << displayTimeAndDate(show->getStartTime())
				<< setw(28) << theatre->getName()
				<< setw(16) << (*bookingsIterator)->getBookedSeats().size()
				<< setw(16) << Enums::getBookingStatusString((*bookingsIterator)->getStatus())
				<< endl;
		}
		util::pressEnterToContinue();
	}
}

/*
 * Function: displayBookingDetails
 * Description: Displays detailed information for a booking selected by ID.
 * Parameters:
 *    None
 * Returns:
 *    void
 */
void UserInterface::displayBookingDetails()
{
	const vector<const Booking*>  bookings = m_controller->getAllBookings();
	if (bookings.empty())
	{
		cout << "No bookings available to display!" << endl;
		util::pressEnterToContinue();
		util::clear();
		return;
	}
	viewAllBookings();
	const vector<string> bookingIds = m_controller->getAllBookingIds();
	string bookingId;
	util::readValueWithRetry(bookingId, "Enter the booking id to see details for: ");
	bool isBookingIdValid = false;
	for (vector<string>::const_iterator iterator = bookingIds.begin(); iterator != bookingIds.end(); ++iterator)
	{
		if ((*iterator) == bookingId)
		{
			isBookingIdValid = true;
			break;
		}
	}
	if (!isBookingIdValid)
	{
		cout << "Booking id is not valid!" << endl;
		util::pressEnterToContinue();
		util::clear();
		return;
	}
	const Booking* booking = m_controller->getBookingById(bookingId);
	if (booking == nullptr)
	{
		cout << "Booking details could not be found!" << endl;
		util::pressEnterToContinue();
		util::clear();
		return;
	}
	displayBookingDetail(booking);
}

/*
 * Function: displayBookingDetail
 * Description: Displays detailed information for a single booking, including Movie, Date,
 *              Theatre, number of seats booked, and booking status.
 * Parameters:
 *    booking (const Booking*) - Pointer to the booking object
 * Returns:
 *    void
 */
void UserInterface::displayBookingDetail(const Booking* booking)
{
	util::clear();
	cout << string(120, '-') << endl;
	cout << left
		<< setw(16) << "Movie ID"
		<< setw(22) << "Movie Name"
		<< setw(22) << "Date"
		<< setw(28) << "Theater Name"
		<< setw(16) << "Screen ID"
		<< setw(16) << "Booked Seats"
		<< setw(16) << "Booking Status"
		<< endl;
	cout << string(120, '-') << endl;
	const Show* show = booking->getShow();
	if (show != nullptr)
	{
		const Movie* movie = show->getMovie();
		const Screen* screen = show->getScreen();
		const Theatre* theatre = nullptr;
		if (screen != nullptr)
		{
			theatre = screen->getTheatre();
		}
		if (movie != nullptr && theatre != nullptr)
		{
			cout << left
				<< setw(16) << movie->getMovieId()
				<< setw(22) << movie->getTitle()
				<< setw(22) << displayTimeAndDate(show->getStartTime())
				<< setw(28) << theatre->getName()
				<< setw(16) << screen->getScreenId()
			    << setw(16) << Enums::getBookingStatusString(booking->getStatus())
				<< endl;
		}
		cout << "Booked Seats: " << endl;
		const vector<string> seatIds = m_controller->getSeatIdsFromBooking(booking);
		for (vector<string>::const_iterator iterator = seatIds.begin(); iterator != seatIds.end(); ++iterator)
		{
			cout << *iterator << endl;
		}
	}
	util::pressEnterToContinue();
}

/*
 * Function: cancelBooking
 * Description: Allows customers to view cancellable bookings, validate booking ID, and cancel a booking.
 * Parameters:
 *    None
 * Returns:
 *    void
 */
void UserInterface::cancelBooking()
{
	string bookingId;
	const vector<const Booking*> bookings = m_controller->getCancellableCustomerBookings();
	displayCustomerBookings(bookings);
	if (bookings.empty())
	{
		return;
	}
	util::readValueWithRetry(bookingId, "Enter Booking Id of booking to cancel: ");
	bool isBookingIdValid = false;
	for (vector<const Booking*>::const_iterator iterator = bookings.begin(); iterator != bookings.end(); ++iterator)
	{
		if ((*iterator) != nullptr && (*iterator)->getBookingId() == bookingId)
		{
			isBookingIdValid = true;
			break;
		}
	}
	if (!isBookingIdValid)
	{
		cout << "Invalid Booking Id" << endl;
		util::pressEnterToContinue();
		util::clear();
		return;
	}
	Enums::ProcessStatus cancelStatus = m_controller->cancelBooking(bookingId);
	string displayMessage = (cancelStatus == Enums::ProcessStatus::SUCCESS) ? "Booking cancelled successfully and payment refunded!" : "Failed to cancel Booking!";
	cout << displayMessage << endl;
	util::pressEnterToContinue();
	util::clear();
}

/*
* Function Name : createBooking
* Description   : Allows the customer to select a movie, choose a show,
*                 select seats, create a booking, and initiate payment.
* Parameters    : None
* Return Type   : void
*/
void UserInterface::createBooking()
{
	string showId, movieId;
	Enums::ProcessStatus status = listShowsForAMovie(movieId, showId);
	if (status == Enums::ProcessStatus::FAILED)
	{
		return;
	}
	const Show* show = m_controller->getShowById(showId);
	if (show == nullptr)
	{
		cout << "Failed to get show seat layout!" << endl;
		util::pressEnterToContinue();
		util::clear();
		return;
	}
	viewShowSeatLayout(show);
	int numberOfSeats;
	util::readValueWithRetry(numberOfSeats, "Enter the number of seats to book (1 - 10):");
	validateNumberOfSeats(numberOfSeats);
	vector<string> bookedSeatIds;
	selectSeats(numberOfSeats, bookedSeatIds, show);
	const Booking* booking = m_controller->bookSelectedSeats(showId, bookedSeatIds);
	if (booking == nullptr)
	{
		cout << "Failed to complete booking!" << endl;
		util::pressEnterToContinue();
		return;
	}
	else
	{
		cout << "Booking completed Successfully!" << endl;
		util::pressEnterToContinue();
	}
	string bookingId = "";
	double amount = 0.0;
	if (booking != nullptr)
	{
		bookingId = booking->getBookingId();
		amount = booking->getAmount();
		cout << "Total booking amount = " << amount << endl;
		initiatePayment(bookingId, amount);
	}
}

/*
* Function Name : validateNumberOfSeats
* Description   : Validates the number of seats entered by the user.
*                 Ensures the value falls within the supported booking limit.
* Parameters    :
*                  numberOfSeats - Number of seats requested for booking
* Return Type   : void
*/
void UserInterface::validateNumberOfSeats(int& numberOfSeats)
{
	bool isNumberOfSeatsValid = false;
	while (!isNumberOfSeatsValid)
	{
		if (numberOfSeats > 0 && numberOfSeats <= 10)
		{
			isNumberOfSeatsValid = true;
		}
		else
		{
			util::readValueWithRetry(numberOfSeats, "Please enter a valid number of seats: ");
		}
	}
}

/*
* Function Name : selectSeats
* Description   : Allows the user to select seats for booking and validates
*                 seat availability before adding them to the booking list.
* Parameters    :
*                  numberOfSeats - Number of seats to be selected
*                  bookedSeatIds - Collection of selected seat IDs
*                  show          - Show for which seats are being booked
* Return Type   : void
*/
void UserInterface::selectSeats(int numberOfSeats, vector<string>& bookedSeatIds, const Show* show)
{
	const ShowSeatAvailability* seatAvailability = show->getSeatAvailability();
	if (seatAvailability == nullptr)
	{
		cout << "Failed to select seats!" << endl;
		util::pressEnterToContinue();
		util::clear();
		return;
	}
	const map<string, Enums::BookingStatus>& seatMap = seatAvailability->getSeatAvailabilityMap();
	string seatId;
	for (int seatIndex = 0; seatIndex < numberOfSeats; ++seatIndex)
	{
		util::readValueWithRetry(seatId, "Enter the seat id of seat number " + to_string(seatIndex + 1) + ": ");
		checkSelectedSeatAvailability(seatMap, seatId, bookedSeatIds);
		bookedSeatIds.push_back(seatId);
	}
}

/*
* Function Name : checkSelectedSeatAvailability
* Description   : Validates whether a selected seat is available for booking.
*                 Prompts the user until an available seat is selected.
* Parameters    :
*                  seatMap - Seat availability map of the show
*                  seatId  - Seat identifier selected by the user
* Return Type   : void
*/
void UserInterface::checkSelectedSeatAvailability(const map<string, Enums::BookingStatus> seatMap, string& seatId, const vector<string>& bookedSeatIds)
{
	bool isSeatAvailable = false;
	while (!isSeatAvailable)
	{
		map<string, Enums::BookingStatus>::const_iterator seat = seatMap.find(seatId);
		if (seat == seatMap.end())
		{
			util::readValueWithRetry(seatId, "Invalid seat id. Please select again: ");
		}
		else if (isSeatAlreadySelected(seatId, bookedSeatIds))
		{
			util::readValueWithRetry(seatId, "The seat is already selected for booking! Please select again:");
		}
		else if (seat->second != Enums::BookingStatus::NOT_BOOKED)
		{
			util::readValueWithRetry(seatId, "The selected seat is not available for booking! Please select again:");
		}
		else
		{
			isSeatAvailable = true;
		}
	}
}

/*
* Function Name : isSeatAlreadySelected
* Description   : Checks whether the specified seat has already been
*                 selected by the user during the current booking process.
* Parameters    :
*                  seatId        - Seat identifier to validate
*                  bookedSeatIds - Collection of already selected seat IDs
* Return Type   : bool
*/
bool UserInterface::isSeatAlreadySelected(const string& seatId, const vector<string>& bookedSeatIds)
{
	for (vector<string>::const_iterator iterator = bookedSeatIds.begin(); iterator != bookedSeatIds.end(); ++iterator)
	{
		if (*iterator == seatId)
		{
			return true;
		}
	}
	return false;
}

void UserInterface::removeMovieFromTheatre()
{
	string theatreId;
	string movieId;
	const vector<const Theatre*> theatres = m_controller->getCurrentOwnerTheatres();
	if (theatres.empty())
	{
		cout << "\nNo theatres found!";
		return;
	}
	displayOwnerTheatres(theatres);
	const vector<string> theatreIds = getTheatreIds(theatres);
	cout << "\nEnter Theatre ID: ";
	util::readValue(theatreId);
	if (validateTheatreId(theatreId, theatreIds) == Enums::ProcessStatus::FAILED)
	{
		cout << "\nInvalid Theatre ID!";
		return;
	}
	const vector<const Movie*> movies = m_controller->getMoviesFromTheatre(theatreId);
	if (movies.empty())
	{
		cout << "\nNo active movies found!";
		return;
	}
	displayMovie(movies);
	const vector<string> movieIds = getMovieIds(movies);
	cout << "\nEnter Movie ID: ";
	util::readValue(movieId);
	if (validateMovieId(movieId, movieIds) == Enums::ProcessStatus::FAILED)
	{
		cout << "\nInvalid Movie ID!";
		return;
	}
	Enums::ProcessStatus status = m_controller->removeMovieFromTheatre(theatreId, movieId);
	if (status == Enums::ProcessStatus::SUCCESS)
	{
		cout << "Movie removed successfully\n";
	}
	else
	{
		cout << "Failed to remove movie\n";
	}
	util::pressEnterToContinue();
}

/*
Function Name : viewRefunds
Description   : Displays all refunds relevant to the authenticated user in a formatted
				tabular view. Retrieves refunds from the controller and prints details
				including Refund ID, Ticket ID, Amount, Status, and Timestamp.
Parameters    : None
Return Type   : void
*/
void UserInterface::viewRefunds()
{
	const vector<Refund*> refunds = m_controller->getRefunds();
	if (refunds.empty())
	{
		cout << "No refunds found." << endl;
		util::pressEnterToContinue();
		return;
	}
	cout << left
		<< setw(12) << "Refund ID"
		<< setw(12) << "Ticket ID"
		<< setw(12) << "Amount"
		<< setw(12) << "Status"
		<< setw(20) << "Time"
		<< endl;
	cout << string(68, '-') << endl;
	for (auto iterator = refunds.begin(); iterator != refunds.end(); ++iterator)
	{
		Refund* refund = *iterator;
		cout << left
			<< setw(12) << refund->getRefundId()
			<< setw(12) << refund->getBookedTicket()->getTicketId()
			<< setw(12) << refund->getRefundAmount()
			<< setw(12) << Enums::getRefundStatusString(refund->getStatus())
			<< setw(20) << util::serializeTime(refund->getTime())
			<< endl;
	}
	util::pressEnterToContinue();
}

void UserInterface::showForceLogoutMessage(bool& isMenuActive)
{
	util::clear();
	cout << "Your account has been deactivated by the Admin. You have been logged out." << endl;
	isMenuActive = false;
	util::pressEnterToContinue();
}

/*
 * Function: ~UserInterface
 * Description: Destructor for the UserInterface class.
 *              Cleans up resources by deleting the associated Controller
 *              instance to prevent memory leaks.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
UserInterface::~UserInterface()
{
	delete m_controller;
}