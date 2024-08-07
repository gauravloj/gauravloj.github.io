---
title: MediaKind
time: 2024-07-26 15:35:40
categories: [Work Experience]
tags:
  [
    c#,
    asp.net,
    docker,
    visual studio,
    typescript,
    javascript,
    google dialogflow,
    google firebase,
    selenium,
    cucumber,
    azure devops,
    grafana,
    kibana,
    elk,
    android sdk,
    linux,
    oauth,
    security services,
    service dashboard,
    ott,
  ]
---

- Designation: Senior Technical Lead
- Term: Oct 2018 - Dec 2021
- Duration: 3 years, 1 months

## Summary

MediaKind is a Global video service provider. Video streaming application on Android OTT and Apple TV are one of its major product.
I joined the company as full stack developer. I have worked on different layers of the product during my time here.
They include security services, backend services, service management dashboard, hardware integration, Android app development, frontend development and testing automation.

## Projects

### Service Dashboard

This is a tenant based portal that allows the user to manage different backend services and related database records all from single website.
These services includes Account management, Catalog management, feature toggles, authentication and authorization, Media metadata settings, Media Access control, routing control and many more.
Being able to manage different services at one place improved project management capabilities for the client.

As a contributor of this dashboard, I have

- Added many feature, such as display of different services mentioned in previous paragraph
- Added functional tests for new features
- Added documentation for different development process to smoothen the onboarding of a new developer in the team
- Lead the development team to ensure timely development of the features and proper testing after deployment

### Voice Integration

Since the main product is a streaming application, **Voice commands** is one of the most liked feature by the users. Approximately 50K subscribers were actively using this feature globally.
This project is about integrating different Voice activated devices to the OTT application.
Main idea is to parse the user's voice command to a compatible format and do the corresponding action on the application UI.
Complexity in this approach comes in using different devices viz. Google home, Google mini, Google Assistant, Alexa, Baidu.

So, my development experience here involved:

- Designing the abstraction layer and integrating these devices to the app
- A lot of research and experiments to come up with a good design that is scalable and maintainable
- Supporting custom voice commands for different clients based on their requirements
- Regular communication with clients to calibrate the Voice settings according to their needs
- Debugging the OTT application using Android SDK
- Collaborate with testers to properly tests the Voice features before release
- Use of Google Cloud services and AWS Lambda to handle text to speech feature of different devices

### Security Services

There are two parts in this project,

- First, generating authorization token for the user that is trying to log in
- Second, create an authorization server to allow logging in via Single Sign-On (SSO) on the application

They may sound similar, but they are for different areas in the product.
Since the production app used SSO login, authorization token is used to track who is logged in and what access privileges do they have inside the application.
On the other hand, the SSO service was used for test environment to emulate the SSO login process in the production application.

My role was to develop the **authorization server** for emulation and parallelly **monitor** the Token generation service and fix any issues that comes up anytime while monitoring.

My responsibilities here includes:

- Develop internal authorization server
- Monitor service health for different backend services
- Investigate any unexpected behavior using tools/techniques like Kibana, host machine logs, local debugging

## My Contributions and Achievements

1. Deep understanding of the system to become an SME for any issues related to the dashboard, voice integration, and authorization server
1. Timely delivery of the features in all the projects
1. Added functional tests to increase the confidence in the code
1. Improved development effort by setting up docker instance locally
1. Added a lot of documentation related to product features, development process, troubleshooting guides
1. Mentored new team members by guiding them during their onboarding phase
1. Design Voice integration services for variety of hardware using Google Cloud Platform and Amazon Web Service
1. Developed in-house SSO service for internal testing
1. Monitor service health on Grafana and investigate any anomalous behavior in the graphs

## Skills Acquired

1. Experience with different cloud services like Google Cloud, AWS
1. Designing scalable distributed systems
1. Working with totally new product, Eg. Use of Google Home for voice integration
1. Full stack development experience
1. Working with Android SDK to debug android apps
1. Deep understanding of underlying infrastructure like CI/CD pipeline, hosting servers, load balancers, API gateways, storage accounts, Identity and Access Management
1. Understanding OAuth + OpenID connect framework for authorization and authentication
1. Understanding underlying security practices for authentication and authorization
1. Collaborating with different teams and stakeholders for a successful delivery of new features
1. Monitor service health using logs aggregation platforms
1. Managing a small team
1. Writing well structured documentations
1. **Tools and Technologies used**:

- Languages: C#, Typescript, Javascript, BASH
- Frameworks: ASP.NET, knockoutjs, Cucumber, OAuth + OpenID connect
- Tools: Visual Studio, Google DialogFlow, Google Firebase, Selenium, Android SDK
- DevOps: Azure pipelines, Docker, Grafana, Kibana + ELK
- Operating Systems: Windows, Linux
