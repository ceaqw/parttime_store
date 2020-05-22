

# 账户信息储存字典
cust_acc = {}		
new_acc_prompt = ["1. Open an account",
				"2. Close an account",
				"3. Deposit money", 
				"4. Withdrawal money", 
				"5. Enquire about balance", 
				"6. Print a list of customers and balances", 
				"7. Quit"]

def func():
	# Event handler
	
	while True:
		
		for i in new_acc_prompt:
			print(i)
	
		cnt = int(input("Please Enter number:"))

		if cnt == 1:
			# Open an account
			
			name = input("Enter Name of the new customer:")
			
			if name in cust_acc:
				print("The amount of money to be deposited")
			else:
				money = int(input("The amount of money to be deposited:"))
				cust_acc[name] = money
			
		elif cnt == 2:
			# Close an account
			
			name = input("Enter the customer/account name to be deleted:")

			if name in cust_acc:
				if cust_acc[name] != 0:
					print("Can not close an account with balance greater than zero")
				else:
					cust_acc.pop(name)
					print("This account has been deleted successfully")
			else:
				print("This customer account doesn’t exist! ") 

		elif cnt == 3:
			# Deposit money
			
			name = input("Enter the customer name for deposit:")

			if name in cust_acc:
				money_before = cust_acc[name]
				money = int(input("Enter the amount to be deposited:"))

				cust_acc[name] += money
				print("your balance is : %d before deposit" % money_before)
				print("your balance is : %d after deposit" % cust_acc[name])
			else:
				print("This customer account doesn’t exist! ")

		elif cnt == 4:
			# Withdrawal money
			 
			name = input("Enter the customer name for withdrawal:")

			if name in cust_acc:
				money = int(input("Enter the amount to be withdrawn:"))

				if money >= cust_acc[name]:
					print("your balance is : %d before withdrawal" % cust_acc[name])
					print("You don't have enough balance to cover the withdrawal")
				else:
					money_before = cust_acc[name]
					cust_acc[name] -= money

					print("your balance is : %d before withdrawal" % money_before)
					print("your balance is : %d after withdrawal" % cust_acc[name])

			else:
				print("This customer account doesn’t exist! ")

		elif cnt == 5:
			# Enquire about balance
			
			name = input("Enter the customer name for Balance enquiry:")

			if name in cust_acc:
				print("your balance is : ", cust_acc[name])
			else:
				print("This customer account doesn’t exist! ")			

		elif cnt == 6:
			# Print a list of customers and balances
			
			print("No Customer Name   Balance")
			print("== =============   =======")

			No = 1
			for i in cust_acc:
				print("%2d " % No ,end="")
				print(i, end="")

				lenstr = len(i)
				num = cust_acc[i]
				lennum = len(str(num))
				num = 23-(lennum+lenstr)

				print(" "*num, end="")
				print("%d" % cust_acc[i])
				No += 1

		elif cnt == 7:
			break
		else:
			print("Unrecognized command")


def main():
	func()

if __name__ == '__main__':
	main()