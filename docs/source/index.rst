.. jajapy documentation master file, created by
   sphinx-quickstart on Thu Jun  2 10:16:49 2022.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to Jajapy's documentation!
==================================

.. image:: pictures/logo.png
   :width: 75 %
   :align: center


**jajapy** is a python library implementing the **Baum-Welch** algorithm on various kinds of **Markov models**.


.. note::
   This project is under active development.


Installation
------------

To use **jajapy**, first install it using pip:

.. code-block:: console

   pip install jajapy

.. note::
   **jajapy** runs on Windows, Linux and MacOS. However, it is **not** multithreaded 
   on Windows and MacOS: hence, we trully recommend to use it on Linux!


Selected Features
-----------------

- Learning HMMs, MCs, MDPs, CTMCs and GoHMMs from traces.
- Parameter estimation for synchronous composition of CTMCs.
- Compatibility with **Prism** and **Storm**.

Content
-------

.. toctree::
   :maxdepth: 2

   References
   help
   tuto
   formal


Source Code
-----------
The source code is available on `github <https://github.com/Rapfff/jajapy>`_.

Contact
-------
``raphal20 at ru dot is``
