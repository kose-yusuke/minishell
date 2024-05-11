// #include <signal.h>

// volatile sig_atomic_t	flag = 0;

// void	handler(int sig)
// {
// 	flag = 1;
// }

// /*
// 	`sigemptyset` initializes the signal set `sa_mask`,
// 	excluding all signals and creating an empty set.

// 	`sigaddset` adds a specific signal (`SIGUSR2` or `SIGUSR1`)
// 	to the designated signal set (`sa_usr1.sa_mask` or `sa_usr2.sa_mask`),
// 	blocking `SIGUSR2` while handling `SIGUSR1`, and vice versa.

// 	`sigaction` assigns the `signal_action` handler to specified signals.
// 	Error handling for `sigaction` is omitted for readability.
// */

// static void	signal_action(int sig, siginfo_t *info, void *ucontext)
// {
// 	(void)ucontext;
// 	// if (g_signal_pid_state == IDLE)
// 	// {
// 	// 	g_signal_pid_state = info->si_pid;
// 	// 	return ;
// 	// }
// 	// if (g_signal_pid_state != info->si_pid)
// 	// 	return ;
// 	// if (sig == SIGUSR1)
// 	// 	g_signal_pid_state = ZERO;
// 	// else if (sig == SIGUSR2)
// 	// 	g_signal_pid_state = ONE;
// }

// void	init_sigaction(void (*signal_action)(int, siginfo_t *, void *))
// {
// 	struct sigaction	sa_usr1;
// 	struct sigaction	sa_usr2;

// 	ft_bzero(&sa_usr1, sizeof(sa_usr1));
// 	ft_bzero(&sa_usr2, sizeof(sa_usr2));
// 	sa_usr1.sa_sigaction = signal_action;
// 	sa_usr2.sa_sigaction = signal_action;
// 	sa_usr1.sa_flags = SA_SIGINFO;
// 	sa_usr2.sa_flags = SA_SIGINFO;
// 	sigemptyset(&sa_usr1.sa_mask);
// 	sigemptyset(&sa_usr2.sa_mask);
// 	sigaddset(&sa_usr1.sa_mask, SIGUSR2);
// 	sigaddset(&sa_usr2.sa_mask, SIGUSR1);
// 	sigaction(SIGUSR1, &sa_usr1, NULL);
// 	sigaction(SIGUSR2, &sa_usr2, NULL);
// }
