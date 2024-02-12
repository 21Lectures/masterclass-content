# Bitcoin Masterclass

## Presentations

In the `presentations/` folder, you will find the slides. To view the `.html` files, simply open them locally with your browser of choice.

## Course exercises

To streamline the virtual machine setup, we access the exercise notebooks through a Docker container with all the necessary dependencies installed.

### Prerequisites
You need
1. [git](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git)
1. [docker](https://docs.docker.com/engine/install/ubuntu/) and [docker-compose](https://docs.docker.com/compose/install/)

Make sure that `docker run hello-world` runs through successfully.

### Setup
Either [clone](https://docs.github.com/en/repositories/creating-and-managing-repositories/cloning-a-repository) or [download](https://docs.github.com/en/repositories/working-with-files/using-files/downloading-source-code-archives) this repository.

Afterwards, in the directory where this `README` is located, start the docker container with:
```bash
docker-compose up
```
A line of the output should contain a link of the form `http://127.0.0.1:8888/?token=<token>` that you can open to find the Jupyter notebook.

### If you have never used Jupyter notebooks
Make sure to familiarize yourself a bit with notebooks. Some introductory links you may find helpful:
 - https://towardsdatascience.com/a-beginners-tutorial-to-jupyter-notebooks-1b2f8705888a
 - https://realpython.com/jupyter-notebook-introduction/
 