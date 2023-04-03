# How to Use Git Actions and Doxygen ?

First, add yaml file to Git Actions part. The first step when creating a workflow is to name the workflow and specify which events it will trigger. The `name` argument is used to name the workflow.
The `on` parameter is used to specify the event from which the workflow is triggered.

```
name: CI

on:
  push:
    branches:
      - main
```

We use the `jobs` argument to define and manage jobs, flows, and steps within the Action. Here build is just a tag for us and represents a single job. The `runs-on` argument is used to determine on which platform the job will run.

Along with `uses`, it is specified what the step in the action will do while it is running.

A job consists of a series of tasks. The steps argument is used to specify each step. `Steps` can run commands, setup tasks, or an action. Steps can be given a name. This is not mandatory but is an important point to make the configuration easier to read.

```
jobs:
  build:
    runs-on: windows-latest
    steps:
      - uses: actions/checkout@v2

      - name: Install Qt
        uses: jurplel/install-qt-action@v3
        with:
          version: "5.12.10"
          host: "windows"
          target: "desktop"
          arch: "win64_mingw73"
          dir: "${{github.workspace}}/"
          install-deps: "true"
```

We use the `run` argument to run the steps directly on the command line.

```
      - name: Build
        run: qmake && make   
```
  
When a workflow runs, the result is as above. The failure of the workflow indicates that we need to reconsider our code.

![image](https://user-images.githubusercontent.com/108115790/229426788-77126fb9-2bee-4f8c-a51d-6bb51e781fa0.png)

## Doxygen
Doxygen provides detailed information about the functions, variables, classes and functions of the code, with the documentation it generates from the source code.

First, download the `Doxygen GUI` for create Doxygen file for the project. With this GUI, we create a path for Doxyfile in our project.

[[https://www.doxygen.nl/download.html]]

![doxy](https://user-images.githubusercontent.com/108115790/229426850-6837305a-7449-4e68-89b2-f774a0fafcc2.PNG)

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

When we download the gh-pages branch as a zip, we open the "index.html" file in it.

![Capturewtrgf](https://user-images.githubusercontent.com/108115790/229426899-de0e0ecc-bd3b-432e-8b74-f2094321e246.PNG)

With the comment in the format specified in our source code, we can explain what the code does, specify the parameters, and specify the code author.

![bdfhbdfb](https://user-images.githubusercontent.com/108115790/229426932-b51bdfb8-ad64-43cc-8d17-159d475f0865.PNG)
  
We can see the comments we specify with index.html.

![hxdhrhr](https://user-images.githubusercontent.com/108115790/229426941-5de368f6-3bed-4dbf-83b5-0bef359ed1f2.PNG)

From now on, our gh-pages branch will be automatically updated after every doxygen update.





