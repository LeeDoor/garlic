You can freely edit anything under tests/ directory once i ask for testing-related work.
For any code outside tests/ directory, edit it only when i directly ask you to do so.
Even when i directly ask for non-test code changes, you still need to ask me to apply your changes each time before making them.
When i ask you to make tests of something, unless i ask the opposite, do following: make simple tests to ensure some feature is working; make complex tests to make sure it works safe with other features; make tests with errors to validate them; make location tests; make whitespace-separation tests to make sure new word, operator or any token is correctly whitespace separated.
When i ask you to "fix" tests, it means that i made a refactoring and now tests does not compile. you should make tests fit latest changes and be compileable. if compilation error outside of test cases, your job is done. if tests does not pass, do not change them unless i ask you.
