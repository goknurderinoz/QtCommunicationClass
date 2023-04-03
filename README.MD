# How to Use Git Actions and Doxygen ?

First, add yaml file to Git Actions part. The first step when creating a workflow is to name the workflow and specify which events it will trigger. The `name` argument is used to name the workflow.
The `on` parameter is used to specify the event from which the workflow is triggered.
Along with `uses`, it is specified what the step in the action will do while it is running.

![[Pasted image 20230403083127.png]]

We use the `jobs` argument to define and manage jobs, flows, and steps within the Action. Here build is just a tag for us and represents a single job. The `runs-on` argument is used to determine on which platform the job will run.

![[Pasted image 20230403083213.png]]

A job consists of a series of tasks. The steps argument is used to specify each step. `Steps` can run commands, setup tasks, or an action. Steps can be given a name. This is not mandatory but is an important point to make the configuration easier to read.
We use the `run` argument to run the steps directly on the command line.

![[Pasted image 20230403090312.png]]
  
When a workflow runs, the result is as above. The failure of the workflow indicates that we need to reconsider our code.

![[Pasted image 20230403083243.png]]

## Doxygen
Doxygen provides detailed information about the functions, variables, classes and functions of the code, with the documentation it generates from the source code.

First, download the `Doxygen GUI` for create Doxygen file for the project. With this GUI, we create a path for Doxyfile in our project.

[[https://www.doxygen.nl/download.html]]

![[doxy.png]]

When the doxyfile file is added to our project, we add it to our yaml file to make it available.

```
deploy:

runs-on: ubuntu-latest

steps:

- uses: DenverCoder1/doxygen-github-pages-action@v1.2.0

with:

github_token: ${{ secrets.GITACTIONS_TOKEN }}

branch: gh-pages

folder: docs/html

```

As we mentioned in the branch section, we create a new branch in our repo.

![[rtgstrhsrt.png]]

When we download the gh-pages branch as a zip, we open the "index.html" file in it.

![[Capturewtrgf.png]]

With the comment in the format specified in our source code, we can explain what the code does, specify the parameters, and specify the code author.

![[bdfhbdfb.png]]
  
We can see the comments we specify with index.html.

![[hxdhrhr.png]]

From now on, our gh-pages branch will be automatically updated after every doxygen update.




