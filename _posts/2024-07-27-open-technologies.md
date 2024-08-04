---
title: OPEN Technologies
time: 2024-07-27 00:50:42
categories: [Work Experience]
tags:
  [
    python,
    django,
    reactjs,
    javascript,
    terraform,
    git,
    githubactions,
    docker,
    digotalocean cloud service,
    postgres,
    database,
    droplets,
    virtual machines,
    content delivery network,
    tailwindcss,
    jira,
    confluence,
    miro,
    figma,
  ]
---

- Designation: Senior Software Developer
- Term: Mar 2022 - Present
- Duration: 2.5+ years

## Summary

Climate change being the global issue, OPEN Technologies started with the idea of providing
city-shapers (builders, constructors, government officials in housing sector) with the energy data analytics to help them in making right decision during initial phase of construction, retrofit of an existing buildings, or maintenance of existing buildings. I joined here as full stack developer and contributed to different projects related to improving energy usage and reducing maintenance costs of a building.

## Projects

### Affordable Housing Navigator (AHN)

This is a website that generates a financial plan for construction of a building based on available government funding and user's expected building design. It helps in making right decision from the beginning of the construction.

This project was in the planning phase when I joined, so it gave me full experience of all the stages of software development lifecycle from requirement gathering to product maintenance.
This project helped generate funding of more than $1 million from Canadian government.

Read more about it on the [official website](https://opentech.eco/products/affordable-housing-navigator/).

### Energy costing Tool

It was initially collection of python scripts to calculate the expected operations cost for the given building. These scripts were written with the intention getting the results instead of using it as part of bigger tool.

I converted these scripts to a python package to have a convenient tool that can be used in any other project.
It uses basic building information and applies a pre-trained surrogate model to predict the energy usage of the building, and using the energy data, different operational cost is calculated based on geographical location, climate zone and other demographical factors.
It is mainly used in AHN website to generate the financial information for a new construction building.

### Decarbonization Tool

This is another product offered by the company that provides a detailed information about building retrofit options.
This tool help in making a retrofit plan for the building that is cost effective and contributes towards the path of net zero.
Similar to AHN, I got to work on this product right from the beginning and apart from the experience of complete software development lifecycle, I got to know more about surrogate models and the science behind the model training and predictions.

Read more about the tool on the [official website](https://opentech.eco/products/virtual-audits-and-decarbonization-planning/).

## My Contributions and Achievements

### AHN

- Setup project repo and CI/CD pipeline for automatic deployment
- Design and setup cloud infrastructure using Terraform
- Developed both frontend and backend from scratch
- Prepared the initial design of the website which helped in getting more funding from the government

### Energy costing Tool

- Refactored the collection of scripts to serve as a single python package
- Optimized the model while refactoring and improved the runtime from 20 minutes to ~15 seconds
- Improved the model to work on both a single dictionary and a tabular data. It reduced the redundant code written for different input types
- Improved the code readability by adding comments and self-descriptive names

### Decarbonization Tool

- Develop the python package to generate decarbonization plans in 1/10th of the time it takes to do it manually
- Designed easy setup of the tool to be used by non-technical person, hence reduced the dependency on developers to generate the final plans

### Overall

- Enhance team productivity by adding project documentation, technical documentation and troubleshooting guides on confluence
- Design system architecture for all the above projects
- Set up the CI/CD pipeline using GithubActions to automatically test and deploy the website on DigitalOcean, thereby generating a quick feedback loop for development
- Upgrade analytics tool to exclude internal traffic from the actual analytics data of the website usage
- Wrote script to generate ssh keys of active development team members, to remove any old unused keys in the clod virtual machines.
- Code reviews to ensure best coding standards

## Skills Acquired

- Complete experience of SDLC: requirement gathering, high/low level designing, planning, development, testing, hosting, maintenance, documentation,
- Experience with different cloud resources viz. virtual machine, database, container registry, CDN, storages, networking, domain service, load balancers, api gateways
- System design experience for a data intensive web application and machine learning tool
- Requirement gathering for a completely new project
- Efficient product maintenance using documentations, and automation testing
- In depth diagnosis of the code to perform runtime optimizations
- DevOps knowledge viz. build pipeline, version control strategies, monitoring tools
- Service health analysis skills to know what to look for and where to look for
- Best security practices
- Tools and Technologies used:
  - Language: Python (Django), Javascript (Reactjs), HTML, CSS (TailwindCSS), BASH
  - DevOps - Terraform, Git, GithubActions, Docker, DO droplets (Virtual Machines), DO Spaces (Content Delivery Network), plausible
  - Database - PostGreSQL database
  - Operating System - Linux
  - Product Management - Jira, Confluence, Miro, Figma
