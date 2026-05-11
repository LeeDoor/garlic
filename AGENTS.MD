Each time you notice some changes here, write me it with first line of your responce.

You can freely edit anything under tests/ directory once i ask for testing-related work.
For any code outside tests/ directory, edit it only when i directly ask you to do so.
Even when i directly ask for non-test code changes, you still need to ask me to apply your changes each time before making them.
When i ask to add, inspect, write, or update tests, do not edit production/source code to make those tests pass. Leave tests failing if the feature is not implemented.
When i ask you to make tests of something, unless i ask the opposite, do following: 
make simple tests to ensure some feature is working; make complex tests to make sure it works safe with other features; 
make tests with errors to validate them; make location tests; 
make whitespace-separation tests to make sure new word, operator or any token is correctly whitespace separated.
When i ask you to "fix" tests, it means that i made a refactoring and now tests does not compile. 
You should make tests fit latest changes and be compileable. if compilation error outside of test cases, your job is done. 
If tests does not pass, do not change them unless i ask you.

When i ask you to make tests for some EBNF rule:
1. Inspect that rule or rules.
2. Write tests with some simple examples that match those rules.
3. Write tests with complicated examples with multiple resolutions of curly brackets, including optional variants and so on. 
4. Write corner cases with no optional parameters at all, with all optional parameters etc.
5. Write tests to check allowed cases of keywords.
6. Write cracking tests with wrong token order, with extra tokens that should not be recognized.
7. Write tests that crack every REQUIREMENTS specified for given rule.
8. Write even more tests at your discretion.

While you are writing tests, never rely on my implementation. always follow the rules in docs/ file i mention.
lexer.ll, parser.yy files account as implementation files.

Never edit any docs i pass you as a ruleset.
When i ask you to make tests, i don't expect them to pass
