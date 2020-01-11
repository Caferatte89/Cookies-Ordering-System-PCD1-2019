/*
Program Name: Cookies Ordering System
Written By: Ong Chen Xiang (DCO2)
Purpose: Enable the bakery to get cookie orders efficiently.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#pragma warning(disable:4996)
#define SHIPPINGPRICE 4.20
#define MEMBERFEES 12.00

// Declare various user-defined function.
void titleLogo(void);
void menuList(void);
int ordering(int orderNum);
void ordernote(int orderNum, char username[14], char isMember, char regMember, char needShipping, int amtLL, int amtNC, int amtAC, double saleLL, double saleNC, double saleAC, double orderTotal, int discount, double discountPrice, double shippingFee, double amtToPay, double deposit);
void registerMember(void);
void changingpassword(void);
void deductpoint(void);
void daySummary(int orderNum);
// Declare a structure for customer's information.
typedef struct
{
	char password[14];
	char name[51];
	char gender;
	char firstTime;
	char phonenumber[13];
	int point;
} memberaccount;

// Global declaration.
int ttlLL = 0, ttlNC = 0, ttlAC = 0;
double ttlSales = 0, ttlDiscount = 0, ttlMemberFees = 0, ttlShippingFees = 0, ttlDeposit = 0;

// Main function.
int main(void)
{
	time_t t = time(NULL);
	char selection, confirmexit, proceed, forChecking;
	int orderNum = 0;
	struct tm tm = *localtime(&t);

	// Call title logo function.
	titleLogo();

	// Display title and date.
	printf("\n     Welcome to using Cookie Ordering System!\n");
	printf("\t     Made by Ong Chen Xiang!\n");
	printf("\t---------------------------------\n");
	printf("\tToday's date: %d/%d/%d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
	// Display main menu.
chooseagain:
	printf("\t---------------------------------\n");
	printf("\t|\t   MAIN  MENU\t\t|\n");
	printf("\t|1. Ordering\t\t\t|\n");
	printf("\t|2. Register new member\t\t|\n");
	printf("\t|3. Changing member password\t|\n");
	printf("\t|4. Deduct member point\t\t|\n");
	printf("\t|5. Summarize today's order\t|\n");
	printf("\t|6. Exit\t\t\t|\n");
	printf("\t---------------------------------");
	printf("\n\tPlease choose a selection > ");
invalidchoice:
	rewind(stdin);
	selection = getchar();
	forChecking = getchar();

	// Validation to ensure it is only digit.
	if (forChecking != '\n')
	{
		printf("\tInvalid selection! Please choose again > ");
		goto invalidchoice;
	}

	// Get input of selection of main menu.
	switch (selection)
	{
	case '1':
		orderNum = ordering(orderNum);
		goto chooseagain;
	case '2':
		printf("\tNew member fees (RM 12.00) will be charged once you make your first order.\n");
		printf("\tProceed? (Y/N) > ");
		do
		{
			rewind(stdin);
			proceed = getchar();
			forChecking = getchar();
			if (toupper(proceed) != 'Y' && toupper(proceed) != 'N' || forChecking != '\n')
				printf("\tInvalid input! Please enter again (Y/N) > ");
		} while (toupper(proceed) != 'Y' && toupper(proceed) != 'N' || forChecking != '\n');
		if (toupper(proceed) == 'Y')
			registerMember();
		goto chooseagain;
	case '3':
		changingpassword();
		goto chooseagain;
	case '4':
		deductpoint();
		goto chooseagain;
	case '5':
		daySummary(orderNum);
		goto chooseagain;
	case '6':
		printf("\tConfirm exit?\n\tAll order history will be erased once you exit. (Y/N) > ");
		do
		{
			rewind(stdin);
			confirmexit = getchar();
			forChecking = getchar();
			if (toupper(confirmexit) != 'Y' && toupper(confirmexit) != 'N' || forChecking != '\n')
				printf("\tInvalid input! Please enter again (Y/N) > ");
		} while (toupper(confirmexit) != 'Y' && toupper(confirmexit) != 'N' || forChecking != '\n');
		if (toupper(confirmexit) == 'N')
			goto chooseagain;
		else
			return 0;
	default:
		printf("\tInvalid selection! Please choose again > ");
		goto invalidchoice;
	}
}

// Function to show title logo.
void titleLogo(void)
{
	printf("\t-----------------------------------\t       *********\n");
	printf("\t|  TARUC FESTIVE COOKIES SDN BHD  |\t    **           **\n");
	printf("\t|  ---COOKIES ORDERING SYSTEM---  |\t  **   *       *   **\n");
	printf("\t-----------------------------------\t *                   *\n");
	printf("\t\t\t|  |\t\t\t *      *            *\n");
	printf("\t\t\t|  |\t\t\t *  *                *\n");
	printf("\t\t\t|  |\t\t\t *              *    *\n");
	printf("\t\t\t|  |\t\t\t *                   *\n");
	printf("\t\t\t|  |\t\t\t  **     *         **\n");
	printf("\t\t\t|  |\t\t\t    **       *   **\n");
	printf("\t\t\t|__|\t\t\t       *********......mhmm cookie! (´.ω.`)...\n");
}
// Function to show menu and discount list.
void menuList(void)
{
	printf("\n------------------------------------------\t----------------------------------------------------------------\n");
	printf("|  COOKIES TYPE\t\t       PRICE     |\t|DISCOUNT:                                                     |\n");
	printf("|  ============\t\t       =====     |\t|========                                                      |\n");
	printf("|  Love Letters\t\tRM 25.00 per tin.|\t|5%% for orders totaling between RM 500 and RM 1000. (inclusive)|\n");
	printf("|  Nut  Cookies\t\tRM 20.00 per jar.|\t|10%% for orders totaling more than RM 1000.                    |\n");
	printf("|Assorted Cookies\tRM 15.00 per tin.|\t|Additional 5%% will be given for member.\t\t       |\n");
	printf("------------------------------------------\t----------------------------------------------------------------\n");
	printf(" *Shipping fees is excluded from price and will be charged if require shipping. (RM 4.20 per 50 units)\n");
	printf(" *Member can be registered and login immediately! (RM 12.00 for registration fees)\n");
	printf(" *Press any key to continue...");
	_getch();
	printf("\n");
}
// Function to make ordering.
int ordering(int orderNum)
{
	FILE *f;
	memberaccount m;
	char isMember, regMember, forChecking, needShipping, confirmOrder, nextOrder;
	char username[14], password[13];
	double saleLL, saleAC, saleNC, discountPrice, shippingFee, orderTotal, amtToPay, deposit;
	int amtLL, amtAC, amtNC, discount, pwcounter;

headstart:
	menuList();

	// +1 to order number for next order.
	orderNum++;

	printf("\n\tOrder No: %d\n", orderNum);
	printf("\t========\n\n");
	printf("\t    Item    \t   Price per unit   \tQuantity\t    Total    \n");
	printf("\t============\t====================\t========\t=============\n");

	// Set number to fulfill below condition for ensure user input integer.
	amtLL = amtAC = amtNC = -1;

	// Get total price amount of Love Letters.
	printf("\tLOVE LETTERS\t<<RM 25.00 per tin>>\t");
	rewind(stdin);
	scanf("%d%c", &amtLL, &forChecking);

	// Set condition to make sure input is in digit.
	while (amtLL <= -1 || forChecking != '\n' || amtLL > 9999)
	{
		if (amtLL <= -1 || forChecking != '\n')
			printf("\t Invalid input! Please enter again >\t");
		if (amtLL > 9999)
			printf("\t Overload! Please enter below 9999 >\t");
		amtLL = -1;
		rewind(stdin);
		scanf("%d%c", &amtLL, &forChecking);
	}
	saleLL = amtLL * 25.0;
	printf("\t\t\t%9d * RM 25.00\t----\t\tRM %10.2f\n", amtLL, saleLL);

	// Get total price amount of Nut Cookies.
	printf("\tNUT  COOKIES\t<<RM 20.00 per jar>>\t");
	rewind(stdin);
	scanf("%d%c", &amtNC, &forChecking);

	// Set condition to make sure input is in digit.
	while (amtNC <= -1 || forChecking != '\n' || amtNC > 9999)
	{
		if (amtNC <= -1 || forChecking != '\n')
			printf("\t Invalid input! Please enter again >\t");
		if (amtNC > 9999)
			printf("\t Overload! Please enter below 9999 >\t");
		amtNC = -1;
		rewind(stdin);
		scanf("%d%c", &amtNC, &forChecking);
	}
	saleNC = amtNC * 20.0;
	printf("\t\t\t%9d * RM 20.00\t----\t\tRM %10.2f\n", amtNC, saleNC);

	// Get total price amount of Assorted Cookies.
	printf("\tASST COOKIES\t<<RM 15.00 per jar>>\t");
	rewind(stdin);
	scanf("%d%c", &amtAC, &forChecking);

	// Set condition to make sure input is in digit.
	while (amtAC <= -1 || forChecking != '\n' || amtAC > 9999)
	{
		if (amtAC <= -1 || forChecking != '\n')
			printf("\t Invalid input! Please enter again >\t");
		if (amtAC > 9999)
			printf("\t Overload! Please enter below 9999 >\t");
		amtAC = -1;
		rewind(stdin);
		scanf("%d%c", &amtAC, &forChecking);
	}
	saleAC = amtAC * 15.0;
	printf("\t\t\t%9d * RM 15.00\t----\t\tRM %10.2f\n", amtAC, saleAC);

	// Get total price amount of all orders.
	printf("\t\t\t\t\t\t\t\t-------------\n");
	orderTotal = saleLL + saleNC + saleAC;
	printf("\t\t\t\t\tORDER TOTAL\t:\tRM %10.2f\n", orderTotal);

	// Set discount condition.
	orderTotal > 1000 ? (discount = 10) : (orderTotal <= 1000 && orderTotal > 500 ? (discount = 5) : (discount = 0));

typeallagain:
	// Get input of is member or not.
	printf("\tMember? (Y/N) > ");
	rewind(stdin);
	isMember = getchar();
	forChecking = getchar();

	// Make condition that ensure user input only Y or N.
	while (toupper(isMember) != 'Y' && toupper(isMember) != 'N' || forChecking != '\n')
	{
		printf("\tInvalid input! Please enter again (Y/N) > ");
		rewind(stdin);
		isMember = getchar();
		forChecking = getchar();
	}

	// Set if it is member, then there is no registration of member.
	if (toupper(isMember) == 'Y')
	{
		regMember = 'N';

	login:
		// Username input.
		printf("\t\t> Username: ");
		rewind(stdin);
		scanf("%13s", username);
		// Validation of username input.
		while (strlen(username) < 7 || strlen(username) > 12 || (f = fopen(username, "r")) == NULL)
		{
			if (strlen(username) < 7 || strlen(username) > 12)
				printf("\t\tInvalid! Username should between 7 and 12 characters.\n");
			else if ((f = fopen(username, "r")) == NULL)
				printf("\t\tUsername didn't existed!\n");
			goto typeallagain;
		}

		// Read password from username.
		f = fopen(username, "r");
		fread(&m, sizeof(m), 1, f);

		// Password input.
		printf("\t\t> Password: ");
		pwcounter = 0;
		rewind(stdin);
		while (pwcounter <= 13)
		{
			password[pwcounter] = getch();
			if (password[pwcounter] == '\b')
			{
				if (pwcounter != 0)
				{
					printf("\b \b");
					pwcounter--;
				}
			}
			else if (password[pwcounter] == '\n' || password[pwcounter] == '\r')
				break;
			else if (pwcounter >= 12)
				continue;
			else
			{
				if (pwcounter < 12)
					printf("*");
				pwcounter++;
			}
		}
		password[pwcounter] = '\0';
		//Validation of password input.
		if (strcmp(password, m.password) != 0)
		{
			printf("\n\t\tInvalid password!\n");
			goto typeallagain;
		}
		else
			isMember = 'Y';
		printf("\n");
		fclose(f);
	}
	else
	{
		printf("\tRegister as new member? (RM 12.00) (Y/N) > ");
		rewind(stdin);
		regMember = getchar();
		forChecking = getchar();

		// Make condition that ensure user input only Y or N.
		while (toupper(regMember) != 'Y' && toupper(regMember) != 'N' || forChecking != '\n')
		{
			printf("\tInvalid input! Please enter again (Y/N) > ");
			rewind(stdin);
			regMember = getchar();
		}

		if (toupper(regMember) == 'Y')
		{
			registerMember();
			printf("\tPlease login again:\n");
			goto login;
		}
	}

	// Add additional 5% discount for member.
	if (toupper(isMember) == 'Y')
		discount += 5;

	// Get input of whether customer needs shipping.
	printf("\tNeeds shipping?  (Y/N) > ");
	rewind(stdin);
	needShipping = getchar();
	forChecking = getchar();

	// Make condition that ensure user input only Y or N.
	while (toupper(needShipping) != 'Y' && toupper(needShipping) != 'N' || forChecking != '\n')
	{
		printf("\tInvalid input! Please type again (Y/N) > ");
		rewind(stdin);
		needShipping = getchar();
	}

	// Calculate shipping fees for customer needs shipping.
	// Shipping Fee = RM 4.20 * ceiling of (total units / 50).
	if (toupper(needShipping) == 'Y')
		shippingFee = SHIPPINGPRICE * ceil((amtLL + amtNC + amtAC) / 50.0);
	else
		shippingFee = 0.0;

	// Calculate the discount price.
	discountPrice = orderTotal * discount / 100.0;

	// Get input of confirming order.
	printf("\tConfirm order? (Y/N) > ");
	rewind(stdin);
	confirmOrder = getchar();
	forChecking = getchar();

	//Make condition that ensure user input only Y or N.
	while (toupper(confirmOrder) != 'Y' && toupper(confirmOrder) != 'N' || forChecking != '\n')
	{
		printf("\tInvalid input! Please type again (Y/N) > ");
		rewind(stdin);
		confirmOrder = getchar();
		forChecking = getchar();
	}

	// Print "Order Cancelled" if order is cancelled.
	if (toupper(confirmOrder) != 'Y')
	{
		printf("\tOrder Cancelled.\n---");
		// Reverse order number for cancelled order.
		orderNum--;

		// Get input for confirmation for order again.
		printf("\n\tOrder again? (Y/N) > ");
		rewind(stdin);
		nextOrder = getchar();
		forChecking = getchar();
		// Make condition that ensure user input only Y or N.
		while (toupper(nextOrder) != 'Y' && toupper(nextOrder) != 'N' || forChecking != '\n')
		{
			printf("\tInvalid input! Please type again (Y/N) > ");
			rewind(stdin);
			nextOrder = getchar();
		}
		printf("\n");

		if (toupper(nextOrder) == 'Y')
			goto headstart;
		else
			return orderNum;
	}
	else
	{
		// Set there are no first time member if there is no registration.
		if (toupper(isMember) == 'N' && toupper(regMember) == 'N')
			m.firstTime = 'N';

		// Displaying discount price.
		printf("\t\t\t\t\tDISCOUNT (%3d%%)\t:\tRM %10.2f\n", discount, discountPrice);

		// Displaying New member fees. (if any)
		if (toupper(regMember) == 'Y' || m.firstTime == 'Y')
			printf("\t\t\t\t\tNEW MEMBER FEES\t:\tRM %10.2f\n", MEMBERFEES);

		// Displaying shipping fees. (if any)
		if (needShipping == 'y' || needShipping == 'Y')
			printf("\t\t\t\t\tSHIPPING FEES\t:\tRM %10.2f\n", shippingFee);

		// Calculate and displaying amount to pay.
		if (toupper(regMember) == 'Y' || m.firstTime == 'Y')
			amtToPay = orderTotal - discountPrice + shippingFee + MEMBERFEES;
		else
			amtToPay = orderTotal - discountPrice + shippingFee;
		printf("\t\t\t\t\tAMOUNT TO PAY\t:\tRM %10.2f\n", amtToPay);

		// Get input of deposit.
		printf("\tAmount of deposit > RM ");
		rewind(stdin);
		scanf("%lf%c", &deposit, &forChecking);

		// Set condition to make sure input is in digit.
		// Set condition to make sure deposit is not more than amount to pay.
		while (deposit <= -1 || forChecking != '\n' || deposit >= amtToPay)
		{
			if (deposit <= -1 || forChecking != '\n')
				printf("\tInvalid input! Please enter again  > RM ");
			if (deposit >= amtToPay)
			{
				printf("\tInvalid! Amount of deposit shouldn't more than amount to pay.\n");
				printf("\tPlease enter again > RM ");
			}
			deposit = -1;
			rewind(stdin);
			scanf("%lf%c", &deposit, &forChecking);
		}
		printf("\t\t\t\t\tDEPOSIT\t\t:\tRM %10.2f\n", deposit);

		// Calculate and displaying balance.
		printf("\t\t\t\t\tBALANCE\t\t:\tRM %10.2f\n\n", amtToPay - deposit);

		// Print order note.
		ordernote(orderNum, username, isMember, regMember, needShipping, amtLL, amtNC, amtAC, saleLL, saleNC, saleAC, orderTotal, discount, discountPrice, shippingFee, amtToPay, deposit);

		// Store total amount of everything.
		ttlLL += amtLL;
		ttlNC += amtNC;
		ttlAC += amtAC;
		ttlSales += orderTotal;
		ttlDiscount += discountPrice;
		ttlDeposit += deposit;
		ttlShippingFees += shippingFee;
		if (m.firstTime == 'Y')
			ttlMemberFees += MEMBERFEES;


		// Get input for confirmation for next order.
		printf("\nNext Order? (Y/N) > ");
		rewind(stdin);
		nextOrder = getchar();
		forChecking = getchar();
		//Make condition that ensure user input only Y or N.
		while (toupper(nextOrder) != 'Y' && toupper(nextOrder) != 'N' || forChecking != '\n')
		{
			printf("Invalid input! Please type again (Y/N) > ");
			rewind(stdin);
			nextOrder = getchar();
		}

		if (toupper(nextOrder) == 'Y')
		{
			goto headstart;
		}
		else
			return orderNum;
	}
}
// Function to display order note.
void ordernote(int orderNum, char username[14], char isMember, char regMember, char needShipping, int amtLL, int amtNC, int amtAC, double saleLL, double saleNC, double saleAC, double orderTotal, int discount, double discountPrice, double shippingFee, double amtToPay, double deposit)
{
	FILE *f = NULL;
	memberaccount m;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	int amountOfDay;

	printf(" ================================================================================================\n");
	printf(" |\t\t\t\t\tORDER    NOTE\t\t\t\t\t\t|\n");
	printf(" |TARUC FESTIVE COOKIES SDN BHD\t\t\t\t\tPhone\t\t: 048995230\t|\n");
	printf(" |Order No: %d\t\t\t\t\t\t\tOrder Date\t: %d/%d/%d\t|\n", orderNum, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

	// Open username file to read username.
	if (toupper(isMember) == 'Y')
	{
		f = fopen(username, "r");
		fread(&m, sizeof(m), 1, f);
		printf(" |Name: %-30s\t\t\t\t\t\t\t\t|\n", m.name);
	}

	// Set there is no first time member if there is no registration.
	if (toupper(isMember) == 'N' && toupper(regMember) == 'N')
		m.firstTime = 'N';
		
	// Calculate amount of days to get order. Amount of days to get order = Order total / 98.75.
	amountOfDay = (int)(orderTotal / 98.75);
	// Set condition to avoid 0 days after calculation.
	if (amountOfDay == 0) amountOfDay++;

	printf(" |Your order will ready in %d day", amountOfDay);
	// Set condition whether to show "day" or "days".
	if (amountOfDay != 1) printf("s");
	printf("!\t\t\t\t\t\t\t\t|\n |\t\t\t\t\t\t\t\t\t\t\t\t|\n");
	printf(" |%10d LOVE LETTERS\t<<RM 25.00 per tin>>\t:\tRM %10.2f\t\t\t|\n", amtLL, saleLL);
	printf(" |%10d  NUT COOKIES\t<<RM 20.00 per tin>>\t:\tRM %10.2f\t\t\t|\n", amtNC, saleNC);
	printf(" |%10d ASST COOKIES\t<<RM 15.00 per tin>>\t:\tRM %10.2f\t\t\t|\n", amtAC, saleAC);
	printf(" |\t\t\t\t\t\t\t\t-------------\t\t\t|\n");
	printf(" |\t\t\t\t\tORDER TOTAL\t:\tRM %10.2f\t\t\t|\n", orderTotal);
	printf(" |\t\t\t\t\tDISCOUNT (%3d%%)\t:\tRM %10.2f\t\t\t|\n", discount, discountPrice);
	// Displaying New credit card fees. (if any)
	if (m.firstTime == 'Y')
		printf(" |\t\t\t\t\tNEW MEMBER FEES\t:\tRM %10.2f\t\t\t|\n", MEMBERFEES);
	// Set condition of show shipping fee if any.
	if (toupper(needShipping) == 'Y')
		printf(" |\t\t\t\t\tSHIPPING FEES\t:\tRM %10.2f\t\t\t|\n", shippingFee);
	printf(" |\t\t\t\t\tAMOUNT TO PAY\t:\tRM %10.2f\t\t\t|\n", amtToPay);
	printf(" |\t\t\t\t\tDEPOSIT PAID\t:\tRM %10.2f\t\t\t|\n", deposit);
	printf(" |\t\t\t\t\tBALANCE\t\t:\tRM %10.2f\t\t\t|\n |\t\t\t\t\t\t\t\t\t\t\t\t|\n", amtToPay - deposit);
	// Ending message.
	printf(" |Thank you for your order! (´ v `)!\t\t\t\t\t\t\t\t|\n");

	// Set it is not first time member anymore.
	if (m.firstTime == 'Y') m.firstTime = 'N';

	if (((int)amtToPay / 10 != 0) && (toupper(isMember) != 'N'))
	{
		// Show how many points has been added.
		printf(" |%d points has been added to your account!\t\t\t\t\t\t", (int)amtToPay / 10);
		if (m.point < 1000)
			printf("\t|\n");
		else
			printf("|\n");
		m.point += (int)amtToPay / 10;
		fclose(f);

		// Update member point.
		f = fopen(username, "w+");
		fwrite(&m, sizeof(m), 1, f);
		fclose(f);

		// Display current point.
		f = fopen(username, "r");
		printf(" |You currently have %d points.\t\t\t\t\t\t\t\t", m.point);
		if (m.point < 100)
			printf("\t|\n");
		else
			printf("|\n");
		fclose(f);
	}
	printf(" |Please bring this Order Note when collecting your cookies. Have a nice day!\t\t\t|\n");
	printf(" ================================================================================================\n");}
// Function to register new member.
void registerMember(void)
{
	FILE *f;
	memberaccount m;
	int pwcounter, v;
	char username[14], confirmpassword[13], confirmation, again, forChecking;

	printf("\n\tREGISTER TO BECOME TARUC FESTIVE COOKIES MEMBER!\n");
	printf("\t================================================\n");

	do
	{
		// Username input.
		printf("\tUsername (7 ~ 12 characters): ");
		rewind(stdin);
		scanf("%13s", username);
		// Validation of username input.
		while (strlen(username) < 7 || strlen(username) > 12 || (f = fopen(username, "r")) != NULL)
		{
			if (strlen(username) < 7 || strlen(username) > 12)
				printf("\tInvalid! Username should between 7 and 12 characters.\n\tType again: ");
			if ((f = fopen(username, "r")) != NULL)
				printf("\tThe username existed!\n\tPlease type again: ");
			rewind(stdin);
			scanf("%13s", username);
		}
		// Create a file named username to store data.
		f = fopen(username, "w");

		// Password input.
		do
		{
			printf("\tPassword (7 ~ 12 characters): ");
		inputpasswordagain:
			pwcounter = 0;
			rewind(stdin);
			while (pwcounter <= 13)
			{
				m.password[pwcounter] = getch();
				if (m.password[pwcounter] == '\b')
				{
					if (pwcounter != 0)
					{
						printf("\b \b");
						pwcounter--;
					}
				}
				else if (m.password[pwcounter] == '\n' || m.password[pwcounter] == '\r')
					break;
				else if (pwcounter >= 12)
					continue;
				else
				{
					if (pwcounter < 12)
						printf("*");
					pwcounter++;
				}
			}
			m.password[pwcounter] = '\0';

			//Validation of password input.
			if (pwcounter < 7)
			{
				printf("\n\tInvalid! Password should more than 7 characters.\n\tType again > ");
				goto inputpasswordagain;
			}

			printf("\n\tConfirmation password: ");
			pwcounter = 0;
			rewind(stdin);
			while (pwcounter <= 13)
			{
				confirmpassword[pwcounter] = getch();
				if (confirmpassword[pwcounter] == '\b')
				{
					if (pwcounter != 0)
					{
						printf("\b \b");
						pwcounter--;
					}
				}
				else if (confirmpassword[pwcounter] == '\n' || confirmpassword[pwcounter] == '\r')
					break;
				else if (pwcounter >= 12)
					continue;
				else
				{
					if (pwcounter < 12)
						printf("*");
					pwcounter++;
				}
			}
			confirmpassword[pwcounter] = '\0';

			// Validation of confimation password.
			if (strcmp(confirmpassword, m.password) != 0)
				printf("\n\tPassword are not same! Please type again.\n");
		} while (strcmp(confirmpassword, m.password) != 0);

		// Full name input.
		printf("\n\tFull Name as in IC: ");
	retypenameagain:
		rewind(stdin);
		gets(m.name);
		// Check validation to ensure user input name only.
		for (v = 0; v < strlen(m.name); v++)
		{
			if (isalpha(m.name[v]) == 0 && (int)m.name[v] != 32 && (int)m.name[v] != 47)
			{
				printf("\tInvalid input! Please enter again > ");
				goto retypenameagain;
			}
			else
				m.name[v] = toupper(m.name[v]);
		}

		// Gender input.
		printf("\tGender (M/F): ");
		do
		{
			rewind(stdin);
			m.gender = getchar();
			forChecking = getchar();
			if (toupper(m.gender) != 'M' && toupper(m.gender) != 'F' || forChecking != '\n')
				printf("\tInvalid! Please enter gender again (M/F): ");
		} while (toupper(m.gender) != 'M' && toupper(m.gender) != 'F' || forChecking != '\n');

		// Phone number input.
		printf("\tContact number (No dash \"-\"): ");
	retypephoneagain:
		rewind(stdin);
		scanf("%12s", m.phonenumber);
		// Validation to ensure user input digit for phone number only.
		for (v = 0; v < strlen(m.phonenumber); v++)
		{
			if (isdigit(m.phonenumber[v]) == 0 || strlen(m.phonenumber) > 11)
			{
				printf("\tInvalid input!\n\tPlease enter contact number again: ");
				goto retypephoneagain;
			}
		}

		// Set point to zero.
		m.point = 0;

		// Set to 'Y' for charge fees when first time order.
		m.firstTime = 'Y';

		// Closing the file.
		fwrite(&m, sizeof(m), 1, f);
		fclose(f);
		again = 'N';

		// Get confirmation from user.
		printf("\tConfirm? (Y/N) > ");
		rewind(stdin);
		confirmation = getchar();
		forChecking = getchar();

		// Make condition that ensure user input only Y or N.
		while (toupper(confirmation) != 'Y' && toupper(confirmation) != 'N' || forChecking != '\n')
		{
			printf("\tInvalid input! Please enter again (Y/N) > ");
			rewind(stdin);
			confirmation = getchar();
			forChecking = getchar();
		}

		// Delete the file if get 'N' from user and get input for start again.
		if (toupper(confirmation) != 'Y')
		{
			remove(username);
			printf("\tCancelled. Start again? (Y/N) > ");
			rewind(stdin);
			again = getchar();
			// Make condition that ensure user input only Y or N.
			while (toupper(again) != 'Y' && toupper(again) != 'N' || forChecking != '\n')
			{
				printf("\tInvalid input! Please enter again (Y/N) > ");
				rewind(stdin);
				again = getchar();
				forChecking = getchar();
			}
			printf("\n");
		}

	} while (toupper(again) == 'Y');
	if (toupper(confirmation) == 'Y')
		printf("\tRegister successful!\n");
	printf("\t================================================\n\n");
}
// Funtion to changing member password.
void changingpassword(void)
{
	FILE *f;
	memberaccount m;
	int pwcounter;
	char username[14], password[13], confirmation, confirmpassword[13], forChecking;

	printf("\n\t---\n");
	// Enter username.
typeallagain:
	printf("\tPlease enter your username (Type E for exit): ");
	rewind(stdin);
	scanf("%13s", username);
	// Validation of username input.
	while (strlen(username) < 7 || strlen(username) > 12 || (f = fopen(username, "r")) == NULL)
	{
		if (toupper(username[0]) == 'E')
		{
			printf("\t---\n");
			return;
		}
		if (strlen(username) < 7 || strlen(username) > 12)
			printf("\tInvalid! Username should between 7 and 12 characters.\n\tType again: ");
		if ((f = fopen(username, "r")) == NULL)
			printf("\tThe username didn't exist!\n\tPlease type again: ");
		rewind(stdin);
		scanf("%13s", username);
	}

	f = fopen(username, "r");
	fread(&m, sizeof(m), 1, f);

	// Password input.
	printf("\tOld Password: ");
	pwcounter = 0;
	rewind(stdin);
	while (pwcounter <= 13)
	{
		password[pwcounter] = getch();
		if (password[pwcounter] == '\b')
		{
			if (pwcounter != 0)
			{
				printf("\b \b");
				pwcounter--;
			}
		}
		else if (password[pwcounter] == '\n' || password[pwcounter] == '\r')
			break;
		else if (pwcounter >= 12)
			continue;
		else
		{
			if (pwcounter < 12)
				printf("*");
			pwcounter++;
		}
	}
	password[pwcounter] = '\0';
	//Validation of password input.
	if (strcmp(password, m.password) != 0)
	{
		printf("\n\tInvalid password!\n");
		goto typeallagain;
	}

	// New Password input.
	do
	{
		printf("\n\t---\n\tNew Password (7 ~ 12 characters): ");
	inputnewpasswordagain:
		pwcounter = 0;
		rewind(stdin);
		while (pwcounter <= 13)
		{
			m.password[pwcounter] = getch();
			if (m.password[pwcounter] == '\b')
			{
				if (pwcounter != 0)
				{
					printf("\b \b");
					pwcounter--;
				}
			}
			else if (m.password[pwcounter] == '\n' || m.password[pwcounter] == '\r')
				break;
			else if (pwcounter >= 12)
				continue;
			else
			{
				if (pwcounter < 12)
					printf("*");
				pwcounter++;
			}
		}
		m.password[pwcounter] = '\0';

		//Validation of new password input.
		if (pwcounter < 7)
		{
			printf("\n\tInvalid! Password should more than 7 characters.\n\tType again > ");
			goto inputnewpasswordagain;
		}

		printf("\n\tConfirmation new password: ");
		pwcounter = 0;
		rewind(stdin);
		while (pwcounter <= 13)
		{
			confirmpassword[pwcounter] = getch();
			if (confirmpassword[pwcounter] == '\b')
			{
				if (pwcounter != 0)
				{
					printf("\b \b");
					pwcounter--;
				}
			}
			else if (confirmpassword[pwcounter] == '\n' || confirmpassword[pwcounter] == '\r')
				break;
			else if (pwcounter >= 12)
				continue;
			else
			{
				if (pwcounter < 12)
					printf("*");
				pwcounter++;
			}
		}
		confirmpassword[pwcounter] = '\0';

		// Validation of confimation password.
		if (strcmp(confirmpassword, m.password) != 0)
			printf("\n\tPassword are not same! Please type again.\n");
	} while (strcmp(confirmpassword, m.password) != 0);

	fclose(f);

	// Get confirmation from user.
	printf("\n\tConfirm? (Y/N) > ");
	rewind(stdin);
	confirmation = getchar();
	forChecking = getchar();

	// Make condition that ensure user input only Y or N.
	while (toupper(confirmation) != 'Y' && toupper(confirmation) != 'N' || forChecking != '\n')
	{
		printf("\tInvalid input! Please enter again (Y/N) > ");
		rewind(stdin);
		confirmation = getchar();
		forChecking = getchar();
	}

	if (toupper(confirmation) == 'Y')
	{
		f = fopen(username, "w");
		fwrite(&m, sizeof(m), 1, f);
		fclose(f);

		printf("\tChanging successfully!\n\t---\n");

	}
	else
		printf("\tCancelled!\n\t---\n");
}
// Function to deduct point.
void deductpoint(void)
{
	FILE *f;
	memberaccount m;
	char proceed, forChecking, username[14], confirmation;
	int deduct;

	printf("\tWarning: This selection is to deduct points from member.\n");
	printf("\tDon't proceed otherwise member use points to exchange things.\n");

	// Get input of proceed.
	printf("\tProceed? (Y/N) > ");
	do
	{
		rewind(stdin);
		proceed = getchar();
		forChecking = getchar();
		if (toupper(proceed) != 'Y' && toupper(proceed) != 'N' || forChecking != '\n')
			printf("\tInvalid input! Please enter again (Y/N) > ");
	} while (toupper(proceed) != 'Y' && toupper(proceed) != 'N' || forChecking != '\n');
	if (toupper(proceed) == 'N')
		return;

	// Get input of username.
	printf("\t---\n\tUsername (Type E for exit): ");
	rewind(stdin);
	scanf("%13s", username);
	// Validation of username input.
	while (strlen(username) < 7 || strlen(username) > 12 || (f = fopen(username, "r")) == NULL)
	{
		if (toupper(username[0]) == 'E')
		{
			printf("\t---\n");
			return;
		}
		if (strlen(username) < 7 || strlen(username) > 12)
			printf("\tInvalid! Username should between 7 and 12 characters.\n\tEnter again: ");
		if ((f = fopen(username, "r")) == NULL)
			printf("\tThe username didn't exist!\n\tPlease enter again: ");
		rewind(stdin);
		scanf("%13s", username);
	}

	f = fopen(username, "r");
	fread(&m, sizeof(m), 1, f);

	// Display and get input of points that need to deduct.
	printf("\tCurrent points are %d.", m.point);
	printf("\n\tPlease enter how many points that you want to deduct: ");
	rewind(stdin);
	scanf("%d%c", &deduct, &forChecking);
	while (deduct < 0 || deduct > m.point || forChecking != '\n')
	{
		if (deduct > m.point)
		{
			printf("\tInvalid input! Point to deduct should less than points that current have!\n");
			printf("\tEnter again: ");
		}
		if (deduct < 0 || forChecking != '\n')
			printf("\tInvalid input! Enter again: ");

		rewind(stdin);
		scanf("%d%c", &deduct, &forChecking);
	}
	m.point -= deduct;
	fclose(f);

	// Get confirmation from user.
	printf("\t---\n\tConfirm? (Y/N) > ");
	rewind(stdin);
	confirmation = getchar();
	forChecking = getchar();

	// Make condition that ensure user input only Y or N.
	while (toupper(confirmation) != 'Y' && toupper(confirmation) != 'N' || forChecking != '\n')
	{
		printf("\tInvalid input! Please enter again (Y/N) > ");
		rewind(stdin);
		confirmation = getchar();
		forChecking = getchar();
	}

	// Deduct points if get confirmation.
	if (toupper(confirmation) == 'Y')
	{
		f = fopen(username, "w");
		fwrite(&m, sizeof(m), 1, f);
		fclose(f);

		printf("\tDeduct successfully!\n\t---\n");

	}
	else
		printf("\tCancelled!\n\t---\n");
}
// Function to display day's summary.
void daySummary(int orderNum)
{
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);

		if (orderNum == 0)
		{
			printf("\n\tNo order recorded!\n");
			return;
		}

		printf("---\n\n=============================================================================================\n");
		printf("\t\t\t\tORDER SUMMARY FOR TODAY (%d/%d/%d)\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
		printf("\t\t\t\t=======================\n");
		printf("\t  Total Number of Customer Order(s) : %d\n", orderNum);
		printf("\t  Total Orders\t\t  Price per unit\t\tAmount\n");
		printf("\t  ============\t\t  ==============\t\t======\n");
		// Calculate and display total amount of cookies for the day.
		printf("%10d LOVE LETTERS\t\t<<RM 25.00 per tin>>\t:\tRM %10.2f\n", ttlLL, ttlLL * 25.0);
		printf("%10d  NUT COOKIES\t\t<<RM 20.00 per tin>>\t:\tRM %10.2f\n", ttlNC, ttlNC * 20.0);
		printf("%10d ASST COOKIES\t\t<<RM 15.00 per tin>>\t:\tRM %10.2f\n", ttlAC, ttlAC * 15.0);
		printf("\t\t\t\tTOTAL SALES\t\t:\tRM %10.2f\n", ttlSales);
		printf("\t\t\t\tTOTAL DISCOUNTS\t\t:\tRM %10.2f\n", ttlDiscount);
		if (ttlMemberFees != 0)
			printf("\t\t\t\tTOTAL MEMBER CARD FEES\t:\tRM %10.2f\t\n", ttlMemberFees);
		printf("\t\t\t\tTOTAL SHIPPING FEES\t:\tRM %10.2f\n", ttlShippingFees);
		printf("\t\t\t\tTOTAL NET SALES\t\t:\tRM %10.2f\n", ttlSales - ttlDiscount + ttlShippingFees + ttlMemberFees);
		printf("\t\t\t\t\t\t\t\t-------------\n");
		printf("\t\t\t\tTOTAL DEPOSITS\t\t:\tRM %10.2f\n", ttlDeposit);
		// Calculate total balance.
		printf("\t\t\t\tTOTAL BALANCE\t\t:\tRM %10.2f\n", ttlSales - ttlDiscount + ttlShippingFees + ttlMemberFees - ttlDeposit);
		printf("=============================================================================================\n");
		printf("Press any key to continue...");
		_getch();
		printf("\n\n");
}