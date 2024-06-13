#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define NAMELENGTH 50 // Group name and User name length = 50
#define NUMBEROFINDIVIDUALS 20 // Number of individuals in input file = 20
#define MAXIMUMNUMBEROFINDIVIDUALSINEACHGROUP 5 // Maximum number of individuals in each group = 5
#define NUMBEROFGROUPS 5

typedef struct UserTag
{
    int UserId;
    char UserName[NAMELENGTH];
    int UserAge;
    int DailyStepGoal;
    int WeeklyStepCount[7];
    int PresentInGroup; // 0 for not present in group, 1 for present in group
    struct UserTag *next;
}User;

typedef struct GroupTag
{
    int GroupId;
    char GroupName[NAMELENGTH];
    int GroupWeeklyGoal;
    User *group_to_user_ptr[MAXIMUMNUMBEROFINDIVIDUALSINEACHGROUP]; // Pointers to users present in group
    struct GroupTag *next;
}Group;

User * CreateUser(int id,char name[],int age,int daily_step_goal,int weekly_step_count[]) // Creates new User with data present in arguments
{
    int i;
    User *userptr = (User *)malloc(sizeof(User));
    if(userptr != NULL)
    {
        userptr->UserId = id;
        strcpy(userptr->UserName,name);
        userptr->UserAge = age;
        userptr->DailyStepGoal = daily_step_goal;
        for(i = 0;i < 7;i++)
        {
            userptr->WeeklyStepCount[i] = weekly_step_count[i];
        }
        userptr->PresentInGroup = 0;
        userptr->next = NULL;
    }
    return userptr;
}

User * AddUser(User* user_head,User *user_ptr) // Places the newly created user node in appropriate place in linked list in sorted order acording to IDs
{
    User *curr_user_ptr,*prev_user_ptr;
    if(user_ptr != NULL)
    {
        if(user_head == NULL) // Empty Linked list
        {
            user_head = user_ptr;
        }
        else
        {
            curr_user_ptr = user_head;
            prev_user_ptr = NULL;
            while((curr_user_ptr != NULL) && (curr_user_ptr->UserId < user_ptr->UserId)) // Traversing the linked list for sorting according to increasing IDs
            {
                prev_user_ptr = curr_user_ptr;
                curr_user_ptr = curr_user_ptr->next;
            }
            if((prev_user_ptr == NULL) && (curr_user_ptr->UserId > user_ptr->UserId)) // Newly created user node's appropriate place is before the head of linked list
            {
                user_ptr->next = user_head;
                user_head = user_ptr;
            }
            else if((curr_user_ptr == NULL) || (curr_user_ptr->UserId > user_ptr->UserId)) // Newly created user node's appropriate place is after the tail of linked list or in between the linked list
            {
                prev_user_ptr->next = user_ptr;
                user_ptr->next = curr_user_ptr;
            }
            else if(curr_user_ptr->UserId == user_ptr->UserId) // If user ID is duplicate
            {
                printf("Duplicate User ID\n");
            }
        }
    }
    return user_head;
}

User * CreateUserList() // Reads data and simultaneously creates new user node and places it in appropriate position in linked list
{
    int i,age,id,daily_step_goal,weekly_step_count[7],j;
    char name[50];
    User *user_head = NULL,*user_ptr = NULL;
    FILE *fptr = fopen("Assignment1DataSample2.txt","r"); //Reads input data from file AssignmentDataSample2.txt opened in read mode
    if(fptr == NULL)
    {
        printf("Failed to open file \n");
    }
    else
    {
        for(i = 0;i < NUMBEROFINDIVIDUALS; i++)
        {
            fscanf(fptr,"%d",&id);
            fscanf(fptr,"%s",name);
            fscanf(fptr,"%d",&age);
            fscanf(fptr,"%d",&daily_step_goal);
            for(j = 0;j < 7; j++)
            {
                fscanf(fptr,"%d",&weekly_step_count[j]);
            }
            user_ptr = CreateUser(id,name,age,daily_step_goal,weekly_step_count); // Creates user node with input data
            if(user_ptr != NULL)
            {
                user_head = AddUser(user_head,user_ptr); // Inserts newly created user in appropriate position in linked list
            }
        }
    }
    fclose(fptr);
    return user_head;
}

void DisplayUser(User *user_ptr) // Displays User Information
{
    int i;
    if(user_ptr != NULL)
    {
        printf("Id : %d \n",user_ptr->UserId);
        printf("Name : %s \n",user_ptr->UserName);
        printf("Age : %d \n",user_ptr->UserAge);
        printf("Daily Step Goal : %d \n",user_ptr->DailyStepGoal);
        printf("Weekly step count : \n");
        for(i = 0;i < 7; i++)
        {
            printf("%d \t",user_ptr->WeeklyStepCount[i]);
        }
        printf("\n\n");
    }
}

User * SearchUserID(User *user_head,int user_id) // Searches user node with given User ID and return its pointer if found
{
    int flag = 0;
    User *user_ptr = user_head,*ret_user_ptr = NULL;
    if(user_head != NULL)
    {
        while(user_ptr != NULL)
        {
            if(user_ptr->UserId == user_id)
            {
                ret_user_ptr = user_ptr;
                flag = 1;
                user_ptr = NULL;
            }
            else
            {
                user_ptr = user_ptr->next;
            }
        }
    }
    if(flag == 0)
    {
        printf("User not found \n");
    }
    return ret_user_ptr;
}

void DisplayUserList(User *user_head) //  Displays information of all users present in user list
{
    User *user_ptr;
    int i;
    if(user_head == NULL)
    {
        printf("No users present \n");
    }
    else
    {
        user_ptr = user_head;
        while(user_ptr != NULL)
        {
            DisplayUser(user_ptr);
            user_ptr = user_ptr->next;
        }
        
    }
}

void UpdateUserListInOutputFile(User *user_head) // Overwrites User List Information in output file
{
    User *user_ptr = user_head;
    user_ptr = user_head;
    FILE *fptr = fopen("Assignment1DataSample3.txt","w");
    if(fptr == NULL)
    {
        printf("Failed to open file \n");
    }
    else
    {
        while(user_ptr != NULL)
        {
            int i;
            fprintf(fptr,"%d \n",user_ptr->UserId);
            fprintf(fptr,"%s \n",user_ptr->UserName);
            fprintf(fptr,"%d \n",user_ptr->UserAge);
            fprintf(fptr,"%d \n",user_ptr->DailyStepGoal);
            for(i = 0;i < 7;i++)
            {
                fprintf(fptr,"%d \n",user_ptr->WeeklyStepCount[i]);
            }
            fprintf(fptr,"\n");
            user_ptr = user_ptr->next;
        }
        fclose(fptr);
    }
}

Group * CreateGroup(int group_id,char group_name[],int group_weekly_goal,User *user_ptr,Group *group_head) // Creates new group with given data as parameters
{
    int i;
    Group *group_ptr,*ret_group_head = NULL,*new_group_ptr = (Group *)malloc(sizeof(Group));
    if(new_group_ptr != NULL)
    {
        new_group_ptr->GroupId = group_id;
        strcpy(new_group_ptr->GroupName,group_name);
        new_group_ptr->GroupWeeklyGoal = group_weekly_goal;
        new_group_ptr->group_to_user_ptr[0] = user_ptr;
        user_ptr->PresentInGroup = 1;
        for(i = 1;i < MAXIMUMNUMBEROFINDIVIDUALSINEACHGROUP;i++)
        {
            new_group_ptr->group_to_user_ptr[i] = NULL;
        }
        new_group_ptr->next = NULL;
        if(group_head == NULL)
        {
            ret_group_head = new_group_ptr;
        }
        else
        {
            ret_group_head = group_head;
            group_ptr = group_head;
            while(group_ptr->next != NULL)
            {
                group_ptr = group_ptr->next;
            }
            group_ptr->next = new_group_ptr;
        }
    }
    return ret_group_head;
}

Group * InsertUserInsideGroup(int group_id,char group_name[],int group_weekly_goal,User *user_ptr,Group *group_head) // Creates a linked list of all groups. Inserts every user's address in group_to_user_ptr array of the given group ID if found else it creates a new group and inserts it there 
{
    Group *group_ptr = group_head,*prev_group_ptr = NULL,*curr_group_ptr = NULL,*new_group_ptr = NULL,*new_group_head = NULL,*ret_group_head = NULL;
    int i; 
    if((user_ptr != NULL) && (user_ptr->PresentInGroup == 0)) // If user not present in group then only function will execute
    {
        if(group_head == NULL) // Empty Group List i.e. no user present in groups
        {
            ret_group_head = CreateGroup(group_id,group_name,group_weekly_goal,user_ptr,group_head);
        }
        else
        {
            while((group_ptr != NULL) && (group_ptr->GroupId != group_id)) // finds the given group ID
            {
                group_ptr = group_ptr->next;
            }
            if(group_ptr == NULL) // Creates a new group ID if group with that particular ID is not found
            {
                ret_group_head = CreateGroup(group_id,group_name,group_weekly_goal,user_ptr,group_head);
            }
            else if(group_ptr->GroupId == group_id) // Group with given Group ID is found 
            {
                i = 0;
                while((i < MAXIMUMNUMBEROFINDIVIDUALSINEACHGROUP) && (group_ptr->group_to_user_ptr[i] != NULL)) // Traverses until no user pointer is present in array
                {
                    i = i + 1;
                }

                if((i < MAXIMUMNUMBEROFINDIVIDUALSINEACHGROUP) && (group_ptr->group_to_user_ptr[i] == NULL)) // If user pointer array is not full
                {
                    group_ptr->group_to_user_ptr[i] = user_ptr;
                    user_ptr->PresentInGroup = 1;
                }
                else // If array is full
                {
                    printf("Group is FULL \n");
                }
                ret_group_head = group_head;
            }
        }
    }
    else
    {
        printf("Entering given user not possible \n");
        ret_group_head = group_head;
    }
    return ret_group_head;
}

Group * CreateGroupList(User *user_head) // Creates List of groups with pointers to its respective users
{
    int i,group_id,weekly_group_goal,group_count,user_id,j;
    User *user_ptr = user_head;
    char group_name[NAMELENGTH];
    Group *group_head = NULL;
    FILE *file_ptr = fopen("GroupDataSample.txt","r"); // Reading group data from input file
    if(file_ptr != NULL)
    {
        for(i = 0;i < NUMBEROFGROUPS;i++)
        {
            fscanf(file_ptr,"%d",&group_id);
            fscanf(file_ptr,"%s",group_name);
            fscanf(file_ptr,"%d",&weekly_group_goal);
            fscanf(file_ptr,"%d",&group_count);
            for(j = 0;j < group_count;j++)
            {
                fscanf(file_ptr,"%d",&user_id);
                user_ptr = SearchUserID(user_head,user_id); // Searches for user pointer to be passed on to InsertUserInsideGroup function
                if(user_ptr != NULL)
                {
                    group_head = InsertUserInsideGroup(group_id,group_name,weekly_group_goal,user_ptr,group_head); // Creates group list
                }
            }
        }
        fclose(file_ptr);
    }
    return group_head;
}

void DisplayGroupList(Group *group_head) // Displays group Information
{
    Group *group_ptr = group_head;
    int i;
    while(group_ptr != NULL)
    {
        printf("\n\nGroup name is : %s \n",group_ptr->GroupName);
        printf("Group ID : %d \n",group_ptr->GroupId);
        printf("Group weekly goal is : %d \n",group_ptr->GroupWeeklyGoal);
        printf("Group Users data is as follows : \n\n\n");
        for(i = 0;((i < MAXIMUMNUMBEROFINDIVIDUALSINEACHGROUP) && (group_ptr->group_to_user_ptr[i] != NULL));i++)
        {
            DisplayUser(group_ptr->group_to_user_ptr[i]);
        }
        group_ptr = group_ptr->next;
    }
    
}

void UpdateGroupListInOutputFile(Group *group_head) // Overwrites information of all groups in output file
{
    int i;
    Group *group_ptr = group_head;
    FILE *file_ptr = fopen("GroupDataSample2.txt","w");
    if(file_ptr != NULL)
    {
        group_ptr = group_head;
        while(group_ptr != NULL)
        {
            fprintf(file_ptr,"%d \n",group_ptr->GroupId);
            fprintf(file_ptr,"%s \n",group_ptr->GroupName);
            fprintf(file_ptr,"%d \n",group_ptr->GroupWeeklyGoal);
            for(i = 0;i < MAXIMUMNUMBEROFINDIVIDUALSINEACHGROUP;i++)
            {
                if(group_ptr->group_to_user_ptr[i] != NULL)
                {
                    fprintf(file_ptr,"%d \n",group_ptr->group_to_user_ptr[i]->UserId);
                }
            }
            fprintf(file_ptr,"\n\n");
            group_ptr = group_ptr->next;
        }
        fclose(file_ptr);
    }
}

int CalculateTotalStepsOfAllUsersInGroup(Group *group_ptr) // returns total number of steps walked by all its users in that particular week
{
    int total_steps = 0,i,j;
    if(group_ptr == NULL)
    {
        printf("Invalid Group ID entered \n");
    }
    else
    {
        for(i = 0;i < MAXIMUMNUMBEROFINDIVIDUALSINEACHGROUP;i++)
        {
            for(j = 0;j < 7;j++)
            {
                if(group_ptr->group_to_user_ptr[i] != NULL)
                {
                    total_steps = total_steps + group_ptr->group_to_user_ptr[i]->WeeklyStepCount[j];
                }
            }
        }
    }
    return total_steps;
}

int FindMaximumUserSteps(User *user_ptr) // Finds and returns maximum steps covered by a user in a single day of a that week
{
    int max_steps = 0,i;
    if(user_ptr != NULL)
    {
        for(i = 0;i < 7;i++)
        {
            if(max_steps < user_ptr->WeeklyStepCount[i])
            {
                max_steps = user_ptr->WeeklyStepCount[i];
            }
        }
    }
    return max_steps;
}

User ** GetTopThree(User *user_head) // Returns base address of array of pointers pointing to Top Three Users
{
    int i,flag,j = 0;
    User *user_ptr = user_head;
    User *DailyGoalCompletedUserList[NUMBEROFINDIVIDUALS];
    User **TopThreeUsers = (User **)malloc(3*(sizeof(User *)));
    if(TopThreeUsers != NULL)
    {
        for(i = 0;i < 3;i++) // Initializing all to NULL
        {
            TopThreeUsers[i] = NULL;
        }
        while(user_ptr != NULL)
        {
            flag = 0;
            for(i = 0;i < 7;i++) // Checks whether a user has completed its daily step goal for each day of the week or not
            {
                if(user_ptr->WeeklyStepCount[i] < user_ptr->DailyStepGoal)
                {
                    flag = 1;
                }
            }
            if(flag == 0) // Adds a user if he has completed the daily goal for each day of the week
            {
                DailyGoalCompletedUserList[j++] = user_ptr;
            }
            user_ptr = user_ptr->next;
        }
        for(i = 0;i < j;i++)
        {
            if(TopThreeUsers[0] == NULL || FindMaximumUserSteps(DailyGoalCompletedUserList[i]) > FindMaximumUserSteps(TopThreeUsers[0])) // Updates top user pointer and pushes the earlier top 2 users down by one places
            {
                TopThreeUsers[2] = TopThreeUsers[1];
                TopThreeUsers[1] = TopThreeUsers[0];
                TopThreeUsers[0] = DailyGoalCompletedUserList[i];
            }
            else if(TopThreeUsers[1] == NULL || FindMaximumUserSteps(DailyGoalCompletedUserList[i]) > FindMaximumUserSteps(TopThreeUsers[1])) // Updates top second user pointer and pushes earlier top second user down by 1 place
            {
                TopThreeUsers[2] = TopThreeUsers[1];
                TopThreeUsers[1] = DailyGoalCompletedUserList[i];
            }
            else if(TopThreeUsers[2] == NULL || FindMaximumUserSteps(DailyGoalCompletedUserList[i]) > FindMaximumUserSteps(TopThreeUsers[2])) // Updates top third user pointer
            {
                TopThreeUsers[2] = DailyGoalCompletedUserList[i];
            }
        }
    }
    return TopThreeUsers;
}

void CheckGroupAchievement(Group *group_head,int group_id) // Checks whether a given group has completed its weekly goal or not
{
    int flag = 0,total_steps = 0,i = 0,j;
    Group *group_ptr = group_head;
    while((group_ptr != NULL) && (flag == 0))
    {
        if(group_ptr->GroupId == group_id)
        {
            flag = 1;
        }
        else
        {
            group_ptr = group_ptr->next;
        }
    }
    if(flag == 0)
    {
        printf("Invalid group ID given \n");
    }
    else
    {
        printf("Total steps of group: %d\n",CalculateTotalStepsOfAllUsersInGroup(group_ptr));
        printf("Group weekly goal: %d\n",group_ptr->GroupWeeklyGoal);
        if(CalculateTotalStepsOfAllUsersInGroup(group_ptr) > group_ptr->GroupWeeklyGoal)
        {
            printf("The group with ID %d has completed its weekly step goal \n",group_id);
        }
        else
        {
            printf("The group with ID %d has not completed its weekly step goal \n",group_id);
        }
    }
}

void CheckIndividualRewards(User *user_head,int user_id) // Allots different number of points to a user if he is presnt in the top 3 users else 0 points are alloted
{
    User *user_ptr = SearchUserID(user_head,user_id);
    User **TopThreeUsers = GetTopThree(user_head);
    int user_points = 0;
    if((user_ptr != NULL) && (TopThreeUsers != NULL))
    {
        if(TopThreeUsers[0] == user_ptr)
        {
            user_points = 100;
        }
        else if(TopThreeUsers[1] == user_ptr)
        {
            user_points = 75;
        }
        else if(TopThreeUsers[2] == user_ptr)
        {
            user_points = 50;
        }
        printf("User %s with ID:%d has %d points \n",user_ptr->UserName,user_ptr->UserId,user_points);
    }
    else
    {
        printf("Invalid user ID entered \n");
    }
}

User * DeleteIndividual(User *user_head,Group **group_head_pptr,int user_id) // Deletes a particular individual from the user linked list and its group
{
    int i,j,flag;
    User *user_ptr,*temp_user_ptr;
    Group *group_head_ptr = *group_head_pptr;
    Group *group_ptr = group_head_ptr;
    user_ptr = SearchUserID(user_head,user_id);
    flag = 0;
    if((user_ptr != NULL) && (group_head_pptr != NULL))
    {
        while (group_ptr != NULL && flag == 0)
        {
            for (i = 0;((i < MAXIMUMNUMBEROFINDIVIDUALSINEACHGROUP) && (group_ptr->group_to_user_ptr[i] != NULL) && (flag == 0)); i++) // Finds user pointer in given group's array of user pointers
            {
                if (group_ptr->group_to_user_ptr[i]->UserId == user_id) // If user to be deleted is found
                {
                    flag = 1;
                    if (i == 0 && group_ptr->group_to_user_ptr[1] == NULL) // If the user to be deleted is the only user present in its group then delete the group from the group list
                    {
                        if (group_ptr == group_head_ptr)
                        {
                            *group_head_pptr = group_ptr->next;
                        }
                        else
                        {
                            Group *prev_group_ptr = group_head_ptr;
                            while (prev_group_ptr->next != group_ptr)
                            {
                                prev_group_ptr = prev_group_ptr->next;
                            }
                            prev_group_ptr->next = group_ptr->next;
                        }
                        printf("Successfully deleted a user from group \n");
                        printf("Successfully deleted a group \n");
                        free(group_ptr);
                    }
                    else // If there are other users also present other than the one to be deleted 
                    {
                        for (j = i;((j < MAXIMUMNUMBEROFINDIVIDUALSINEACHGROUP - 1) && (group_ptr->group_to_user_ptr[j + 1] != NULL)); j++)
                        {
                            group_ptr->group_to_user_ptr[j] = group_ptr->group_to_user_ptr[j + 1];
                        }
                        group_ptr->group_to_user_ptr[j] = NULL;
                        printf("Successfully deleted a user from a group \n");
                    }
                }
            }
            if (flag == 0) // If user is not found
            {
                group_ptr = group_ptr->next;
            }
        }
        if((group_ptr == NULL) && (flag == 0)) // If entire group list is checked and still user is not found
        {
            printf("Deletion not possible \n");
        }
        else
        {
            UpdateGroupListInOutputFile(group_head_ptr);
            if(user_head == user_ptr) // If head of user list is itself the node to be deleted 
            {
                user_head = user_ptr->next;
                free(user_ptr);
                user_ptr = NULL;
                printf("Successfully deleted a user \n");
            }
            else // If any intermediate or last node is to be deleted
            {
                temp_user_ptr = user_head;
                while(temp_user_ptr->next != user_ptr)
                {
                    temp_user_ptr = temp_user_ptr->next;
                }
                if(temp_user_ptr != NULL)
                {
                    temp_user_ptr->next = user_ptr->next;
                    free(user_ptr);
                    user_ptr = NULL;
                    printf("Successfully deleted a user \n");
                }
                else
                {
                    printf(" User not found \n");
                }
            }
            UpdateUserListInOutputFile(user_head);
        }
    }
    return user_head;
}

Group * DeleteGroup(Group *group_head,int group_id) // Deleting a particular group
{
    int flag = 0,i;
    Group *group_ptr = group_head,*prev_group_ptr = NULL,*ret_group_head = group_head;
    while((group_ptr != NULL) && (flag == 0))
    {
        if(group_ptr->GroupId == group_id)
        {
            flag = 1;
        }
        else
        {
            prev_group_ptr = group_ptr;
            group_ptr = group_ptr->next;
        }
    }
    if(flag == 1) // If group is found 
    {
        if(prev_group_ptr == NULL) // If group to be deleted is the group head
        {
            i = 0;
            while(group_ptr->group_to_user_ptr[i] != NULL) // Changing the status of every user of the group to not being present in any group
            {
                group_ptr->group_to_user_ptr[i]->PresentInGroup = 0;
                i = i + 1;
            }
            ret_group_head = group_ptr->next;
            printf("Successfully deleted a group \n");
            free(group_ptr);
        }
        else // If the group to be deleted is intermediate in the list
        {
            i = 0;
            while(group_ptr->group_to_user_ptr[i] != NULL) // Changing the status of every user of the group to not being present in any group
            {
                group_ptr->group_to_user_ptr[i]->PresentInGroup = 0;
                i = i + 1;
            }
            prev_group_ptr->next = group_ptr->next;
            printf("Successfully deleted a group \n");
            free(group_ptr);
        }
    }
    else // Invalid Group ID
    {
        printf("Group not found \n");
    }
    return ret_group_head;
}

void DisplayGroupInformation(Group *group_head,int group_id) // Displays information of a group along with its rank
{
    int i,j = 1;
    Group *group_ptr = group_head;
    if(group_head != NULL)
    {
        while ((group_ptr != NULL) && (group_ptr->GroupId != group_id))
        {
            group_ptr = group_ptr->next;
            j = j + 1;
        }
        if(group_ptr == NULL)
        {
            printf("Given group ID does not exist \n");
        }
        else if(group_ptr->GroupId == group_id)
        {
            printf("Rank: %d \n",j);
            //printf("Group ID:%d\n",group_ptr->GroupId);
            //printf("Group name:%s\n",group_ptr->GroupName);
            printf("Group weekly goal:%d\n",group_ptr->GroupWeeklyGoal);
            printf("User IDs:\n");
            for(i = 0;((group_ptr->group_to_user_ptr[i] != NULL) && (i < MAXIMUMNUMBEROFINDIVIDUALSINEACHGROUP));i++)
            {
                printf("%d\t",group_ptr->group_to_user_ptr[i]->UserId);
            }
            printf("\n");
        }
    }
    else
    {
        printf("Invalid group head pointer \n");
    }
}

void SuggestDailyGoalUpdate(User *user_head,User *user_ptr) // Suggests the minimum daily step goal required to appear in the top 3 users
{
    User **TopThreeUsers = GetTopThree(user_head);
    int average_daily_steps_for_top_three = ((TopThreeUsers[0]->DailyStepGoal + TopThreeUsers[1]->DailyStepGoal + TopThreeUsers[2]->DailyStepGoal) / 3); // Calculates average daily step goal
    int new_daily_step_goal,flag = 0;
    if((user_ptr == TopThreeUsers[0]) || (user_ptr == TopThreeUsers[1]) || (user_ptr == TopThreeUsers[1]))
    {
        printf("User is already present in top three \n");
        printf("Do you still want to change daily step goal. Enter 0 for yes 1 for no. \n");
        scanf("%d",&flag);
    }
    if((user_ptr != NULL) && (flag == 0))
    {
        printf("Minimum daily steps to get into contention of top three is %d \n",average_daily_steps_for_top_three + 1);
        printf("Enter the new daily step goal target %s wants to set: \n",user_ptr->UserName);
        scanf("%d",&new_daily_step_goal);
        user_ptr->DailyStepGoal = new_daily_step_goal;
        UpdateUserListInOutputFile(user_head);
    }
    else
    {
        printf("Invalid user ID entered \n");
    }
}

Group * MergeGroups(Group *group_head,int group_id_1,int group_id_2) // Merging two groups with given group IDs and deleting the original groups
{
    int flag = 0,count1 = 0,count2 = 0,i,j,k;
    Group *group_ptr_1 = NULL,*group_ptr_2 = NULL,*group_ptr = group_head,*ret_group_head = group_head;
    while(group_ptr != NULL)
    {
        if(group_ptr->GroupId == group_id_1) // If group with group ID 1 is found
        {
            group_ptr_1 = group_ptr;
        }
        else if(group_ptr->GroupId == group_id_2) // If group with group ID 2 is found
        {
            group_ptr_2 = group_ptr;
        }
        group_ptr = group_ptr->next;
    }
    if((group_ptr_1 == NULL) && (group_ptr_2 == NULL)) // If both groups are not found
    {
        printf("Invalid group IDs entered \n");
    }
    else if(group_ptr_1 == NULL && group_ptr_2 != NULL) // If group with ID 1 is not found
    {
        printf("Invalid ID 1 entered \n");
    }
    else if(group_ptr_1 != NULL && group_ptr_2 == NULL) // If group with ID 2 is not found
    {
        printf("Invalid ID 2 entered \n");
    }
    else // If both groups are found
    {
        i = 0;
        while(group_ptr_1->group_to_user_ptr[i] != NULL) // Calculating total number of users group 1
        {
            count1 = count1 + 1;
            i = i + 1;
        }
        i = 0;
        while(group_ptr_2->group_to_user_ptr[i] != NULL) // Calculating total number of users group 2
        {
            count2 = count2 + 1;
            i = i + 1;
        }
        if((count1 + count2) > 5) // If total users are more than 5 then groups cannot be merge because maximum users in a group is 5
        {
            printf("Groups cannot be merged \n");
        }
        else // If total number of users are less than 5
        {
            i = j = k = 0;
            Group *merged_group_ptr = (Group *)malloc(sizeof(Group));
            for(i = 0;i < MAXIMUMNUMBEROFINDIVIDUALSINEACHGROUP;i++)
            {
                if(group_ptr_1->group_to_user_ptr[j] != NULL) // Assigning all user pointers in group 1 to new merged group
                {
                    merged_group_ptr->group_to_user_ptr[i] = group_ptr_1->group_to_user_ptr[j];
                    group_ptr_1->group_to_user_ptr[j]->PresentInGroup = 0;
                    j = j + 1;
                }
                else if(group_ptr_2->group_to_user_ptr[k] != NULL) // Assigning all user pointers in group 2 to new merged group
                {
                    merged_group_ptr->group_to_user_ptr[i] = group_ptr_2->group_to_user_ptr[k];
                    group_ptr_2->group_to_user_ptr[k]->PresentInGroup = 0;
                    k = k + 1;
                }
                else // Assigning NULL to the array if addresses of all users present in both groups are present in array
                {
                    merged_group_ptr->group_to_user_ptr[i] = NULL;
                }
            }
            strcpy(merged_group_ptr->GroupName,group_ptr_1->GroupName);
            merged_group_ptr->GroupWeeklyGoal = group_ptr_1->GroupWeeklyGoal + group_ptr_2->GroupWeeklyGoal;
            group_head = DeleteGroup(group_head,group_id_1);
            group_head = DeleteGroup(group_head,group_id_2);
            i = 0;
            while((i < MAXIMUMNUMBEROFINDIVIDUALSINEACHGROUP) && (merged_group_ptr->group_to_user_ptr[i] != NULL)) // Changing the status of all users present in both groups to present in group
            {
                merged_group_ptr->group_to_user_ptr[i]->PresentInGroup = 1;
                i = i + 1;
            }
            merged_group_ptr->next = group_head;
            group_head = merged_group_ptr;
            merged_group_ptr->GroupId = ((group_id_1 * 10) + group_id_2);
            UpdateGroupListInOutputFile(group_head);
            ret_group_head = group_head;
        }
    }
    return ret_group_head;
}

Group *Divide(Group *lptr) // Divides the linked list into two parts
{
    Group *fast,*slow,*ptr;
    slow=lptr;
    fast=lptr->next->next;

    while(fast!=NULL){
        fast=fast->next;
        slow=slow->next;

        if(fast!=NULL){
            fast=fast->next;
        }
    }
    ptr=slow->next;
    slow->next=NULL;
    return ptr;
}

Group *Merge (Group *lptr1,Group *lptr2) // Merges the two sorted divided linked list into a sorted larger linked list
{
    Group *ptr1,*ptr2,*result,*tail;

    if(lptr1==NULL){
        result=lptr2;
    }
    else if(lptr2==NULL){
        result=lptr1;
    }
    else{
        ptr1=lptr1;
        ptr2=lptr2;
        if(CalculateTotalStepsOfAllUsersInGroup(lptr1) > CalculateTotalStepsOfAllUsersInGroup(lptr2)){
            result=lptr1;
            ptr1=ptr1->next;
        }
        else{
            result=lptr2;
            ptr2=ptr2->next;
        }
        tail=result;

        while((ptr1!=NULL)&&(ptr2!=NULL)){
            if(CalculateTotalStepsOfAllUsersInGroup(ptr1) > CalculateTotalStepsOfAllUsersInGroup(ptr2)){
               tail->next=ptr1;
               tail=tail->next;
               ptr1=ptr1->next;
            }
            else{
                tail->next=ptr2;
                tail=tail->next;
                ptr2=ptr2->next;
            }
        }
        if(ptr1!=NULL){
            tail->next=ptr1;
        }
        else{
            tail->next=ptr2;
        }
    }
    return result;
}

Group * MergeSort(Group *lptr) // Sorts the linked list by calling two functions divide and merge
{
    Group * ptr;            
    Group * head =lptr;      

    if((lptr!=NULL) && (lptr -> next!=NULL)){
        ptr=Divide (lptr);
        lptr=MergeSort(lptr);
        ptr=MergeSort(ptr);
        head=Merge(lptr,ptr);
    }
    return head;
}

void DisplayLeaderBoard(Group *group_head) // Displays Group Leaderboard
{
    Group *group_ptr = group_head;
    int i = 1;
    while(group_ptr != NULL)
    {
        printf("\n\n%d. %s \n",i,group_ptr->GroupName);
        printf("Total Steps: %d \n",CalculateTotalStepsOfAllUsersInGroup(group_ptr));
        group_ptr = group_ptr->next;
        i = i + 1;
    }
}

Group * GenerateGroupLeaderBoard(Group *group_head) // Merge sorts the groups using merge sort function according to total steps in descending order
{
    int i;
    Group *group_ptr = group_head,*new_group_ptr = NULL,*new_group_head = NULL,*curr_group_ptr = NULL,*prev_group_ptr = NULL;
    group_head = MergeSort(group_head);
    UpdateGroupListInOutputFile(group_head);
    group_ptr = group_head;
    return group_ptr;
}

User * CheckDuplicateUser(User *user_head,User *user)
{
    int found = 0;
    User *user_ptr = user_head,*ret_user_ptr = NULL;
    while((user_ptr != NULL) && (found == 0))
    {
        if(user_ptr->UserId == user->UserId)
        {
            found = 1;
            ret_user_ptr = user_ptr;
        }
        user_ptr = user_ptr->next;
    }
    return ret_user_ptr;
}

int CheckGroupPresence(Group *group_head,int group_id)
{
    int found = 0;
    Group *group_ptr = group_head;
    while((group_ptr != NULL) && (found == 0))
    {
        if((group_ptr->GroupId == group_id) && (group_ptr->group_to_user_ptr[MAXIMUMNUMBEROFINDIVIDUALSINEACHGROUP-1] == NULL))
        {
            found = 1;
        }
        else if((group_ptr->GroupId == group_id) && (group_ptr->group_to_user_ptr[MAXIMUMNUMBEROFINDIVIDUALSINEACHGROUP-1] != NULL))
        {
            found = 2;
        }
        group_ptr = group_ptr->next;
    }
    return found;
}

int main()
{
    User *user_head,*user,**TopThreeUsers;
    Group *group_head,*new_group_head;
    int group_id,user_id,i,group_id_1,group_id_2,choice_num = 1;
    user_head = CreateUserList();
    //DisplayUserList(user_head);
    UpdateUserListInOutputFile(user_head);
    group_head = CreateGroupList(user_head);
    //DisplayGroupList(group_head);
    UpdateGroupListInOutputFile(group_head);
    printf("\t\t\t\t\t\t\t STEP TRACKING APPLICATION \t\t\n");
    while(choice_num != 0)
    {
        printf("Enter which function number you want to execute: \n");
        printf("\n 0. EXIT \n");
        printf("\n 1. Add Person \n");
        printf("\n 2. Create Group \n");
        printf("\n 3. Get Top 3 \n");
        printf("\n 4. Check Group Achievement \n");
        printf("\n 5. Generate Leaderboard \n");
        printf("\n 6. Check Individual Rewards \n");
        printf("\n 7. Delete Individual \n");
        printf("\n 8. Delete Group \n");
        printf("\n 9. Merge Groups \n");
        printf("\n 10. Display Group Information \n");
        printf("\n 11. Suggest Individual's Daily Goal Update \n\n");
        scanf("%d",&choice_num);
        switch(choice_num)
        {
            case 0:
            {
                printf("\n\nLEAVING THE APPLICATION \n\n");
                break;
            }
            case 1:
            {
                User *user_ptr;
                int weekly_step_count[7] = {10000,2000,3000,4000,6000,5000,8900};
                user = CreateUser(1021,"Rishabh",19,7000,weekly_step_count);
                user_ptr = CheckDuplicateUser(user_head,user);
                if(user_ptr == NULL)
                {
                    printf("Successfuly added a user with name: %s and ID: %d \n",user->UserName,user->UserId);
                    user_head = AddUser(user_head,user);
                }
                UpdateUserListInOutputFile(user_head);
                break;
            }
            case 2:
            {
                int ans;
                user = SearchUserID(user_head,1020);
                ans = CheckGroupPresence(group_head,1);
                group_head = InsertUserInsideGroup(1,"DSA",450000,user,group_head);
                if(ans == 0)
                {
                    printf("New group created and user added \n");
                }
                else if(ans == 1)
                {
                    printf("User added to group \n");
                }
                user = SearchUserID(user_head,1019);
                ans = CheckGroupPresence(group_head,6);
                group_head = InsertUserInsideGroup(6,"Assignemnt1",100000,user,group_head);
                if(ans == 0)
                {
                    printf("New group created and user added \n");
                }
                else if(ans == 1)
                {
                    printf("User added to group \n");
                }
                UpdateGroupListInOutputFile(group_head);
                break;
            }
            case 3:
            {
                TopThreeUsers = GetTopThree(user_head);
                printf("Top three users are: \n");
                for(i = 0;i < 3;i++)
                {
                    if((TopThreeUsers[i]) != NULL)
                    {
                        printf("%d.%s\t\tId:%d\t\tDailyStepGoal:%d\tMaximum Steps:%d \n",i + 1,TopThreeUsers[i]->UserName,TopThreeUsers[i]->UserId,TopThreeUsers[i]->DailyStepGoal,FindMaximumUserSteps(TopThreeUsers[i]));
                    }
                }
                break;
            }
            case 4:
            {
                printf("Enter a group ID to check its achievements: \n");
                scanf("%d",&group_id);
                CheckGroupAchievement(group_head,group_id);
                break;
            }
            case 5:
            {
                // printf("Group 1: %d \n",CalculateTotalStepsOfAllUsersInGroup(group_head));
                // printf("Group 2: %d \n",CalculateTotalStepsOfAllUsersInGroup(group_head->next));
                // printf("Group 3: %d \n",CalculateTotalStepsOfAllUsersInGroup(group_head->next->next));
                // printf("Group 4: %d \n",CalculateTotalStepsOfAllUsersInGroup(group_head->next->next->next));
                // printf("Group 5: %d \n",CalculateTotalStepsOfAllUsersInGroup(group_head->next->next->next->next));
                // printf("Group 6: %d \n",CalculateTotalStepsOfAllUsersInGroup(group_head->next->next->next->next->next));
                new_group_head = GenerateGroupLeaderBoard(group_head);
                DisplayLeaderBoard(new_group_head);
                group_head = new_group_head;
                break;
            }
            case 6:
            {
                printf("Enter User ID to check his/her individual rewards: \n");
                scanf("%d",&user_id);
                CheckIndividualRewards(user_head,user_id);
                break;
            }
            case 7:
            {
                printf("Enter User ID of a user which is to be deleted \n");
                scanf("%d",&user_id);
                DeleteIndividual(user_head,&group_head,user_id);
                UpdateUserListInOutputFile(user_head);
                break;
            }
            case 8:
            {
                printf("Enter ID of group which is to be deleted: \n");
                scanf("%d",&group_id);
                group_head = DeleteGroup(group_head,group_id);
                UpdateGroupListInOutputFile(group_head);
                break;
            }
            case 9:
            {
                printf("Enter IDs of the two groups you want to merge: \n");
                scanf("%d%d",&group_id_1,&group_id_2);
                group_head = MergeGroups(group_head,group_id_1,group_id_2);
                UpdateGroupListInOutputFile(group_head);
                break;
            }
            case 10:
            {
                printf("Enter group ID of a group to display its information: \n");
                scanf("%d",&group_id);
                new_group_head = GenerateGroupLeaderBoard(group_head);
                DisplayGroupInformation(new_group_head,group_id);
                group_head = new_group_head;
                break;
            }
            case 11:
            {
                printf("Enter the user whose daily step goal is to be updated: \n");
                scanf("%d",&user_id);
                user = SearchUserID(user_head,user_id);
                SuggestDailyGoalUpdate(user_head,user);
                break;
            }
            default:
            {
                printf("\n\nInvalid option entered, TRY AGAIN!! \n\n");
                break;
            }
        }
    }
    return 0;
}

