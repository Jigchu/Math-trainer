# This is the homepage for the command-line-interface part of the wiki.

## The Program's Lifecycle

I am not sure if I am using those words correctly but what I mean is all the program's processes from when it is called to when it is terminated. Below if a diagram detailing the program's lifecycle and its interactions with the user.

```mermaid
---
title: The Program's Lifecycle
---
flowchart LR
	subgraph Program
		direction LR
		Start_program[Start]
		Parse[Parse input]
		Gen_Q1[Generates questions]
		Gen_Q2[Generates questions]
		Gen_Q3[Generates questions]
		Qset[Global question set variable]
		Ask_Q[Asks questions]
		Get_input[Recieves input]
		Check_Ans[Checks input]
		Marked_pp[Variable to keep track of wrong and correct answers]
		End_program[End]

		Start_program -->Parse
		Parse -->Gen_Q1
		Parse -->Gen_Q2
		Parse -->Gen_Q3
		Gen_Q1 -->|Places questions|Qset
		Gen_Q2 -->|Places questions|Qset
		Gen_Q3 -->|Places questions|Qset
		Ask_Q -->|Reads questions|Qset
		Get_input -->Check_Ans
		Check_Ans -->|Reads correct answer|Qset
		Check_Ans -->|Updates|Marked_pp
		Check_Ans -->Ask_Q
		Check_Ans -->|References for results after user done answering all questions|Marked_pp
		Check_Ans -->|Misc Stuff|End_program
	end
	
	subgraph User
		direction LR
		Start_user[Start]
		User_sb[On standby]
		Read_Q[Reads questions]
		Ans_Q[Answers question]
		Result_await[Waits for results]
		End_user[End]

		Start_user -->User_sb
		User_sb -->Read_Q
		Read_Q -->Ans_Q
		Ans_Q -->User_sb
		User_sb -->|After finishing all questions|Result_await
		Result_await -->|After receiving results|End_user
	end

	Start_user -->|Calls program|Start_program
	Ask_Q -->User_sb
	Ans_Q-->|Inputs answer|Get_input
	Check_Ans -->|Resturns results|Result_await
```

## Docs

To get started you can look through the docs to understand more about the program. You should first start with [**parsing the options**](https://github.com/Jigchu/Math-trainer/wiki/Options)