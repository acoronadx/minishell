This tester works in some minishells and some not.
1.uncompress in minishell area and make make of minishell.
2.minishell program has to be called "minishell"
3.go inside Tester_Japo folder and execute ./Japotester. Make it chmod +x if necessary.


The tester won't test all aspect of minishell but will use the redirect method to compare
"ls > file" so it needs to be implemented in your minishells.

As said, in some minishell don't work because I think is implemented (istty) in a manner that cannot accept arguments in ./minishell
necesary to work.

A red X doesn't mean is wrong in your tester. Only points you something to test with bash. as for
example you can print "invalid command" and in bash could be "not a valid command". Just notify possible errors.

The tester was made only for our minishell so it checks deeply echo function, exit, expanse vars and "" or ''
Enjoy.
