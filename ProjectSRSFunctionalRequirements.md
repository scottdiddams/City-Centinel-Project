# CitySentinel

## Software Requirements Specification

### Functional Requirements

#### System Administrator

1.
 - ID: FR-SYSADMIN01
 - TITLE: Full System Access
 - DESCRIPTION: The System administrator shall have access to all abilities described below
 - RATIONALE: The system administrator has full ownership and control of the application and cannot be denied access to any information within the system
 - DEPENDENCIES: ALL

#### Administrator

1.  
 - ID: FR-ADMIN01
 - TITLE: Manage User permissions
 - DESCRIPTION: An application administrator shall have the ability to approve requests to add, remove, and modify permissions of user entities in the active directory. modify permissions is defined as enable particular form access & approve device additions to the system.
 - RATIONALE: Users will require administrator approval for use of the application to avoid disorganization or redundant user entities
 - DEPENDANCIES: TBD
 
2. 
 - ID: FR-ADMIN02
 - TITLE: Unlimited Input Monitoring
 - DESCRIPTION: An application administrator shall have the ability to view, partition, and quarentine input data
 - RATIONALE: To ensure data quality, an administrator views all data to identify corrupting data - partition that data, and remove it from the functional relation and into "Quarentine" for troubleshooting & error analysis
 - DEPENDENCIES: TBD
 
3. 
 - ID: FR-ADMIN03
 - TITLE: Unlimited Data Download
 - Description: An application administrator shall have the ability to download all input data
 - RATIONALE: To allow analysis and quality assurance, the administrator has access to everything the user does
 -DEPENDANCIES: TBD

4. 
 - ID: FR-ADMIN04
 - TITLE: Feedback form access and use
 - DESCRIPTION: An application administrator shall have access to application feedback forms and the ability to respond to user requests for information and assistance
 - RATIONALE: To allow all administrators to handle customer help tickets and respond to users
 - DEPENDENCIES: TBD
 
5. 
 - ID: FR-ADMIN05
 - TITLE: Universal Cloud to Device (C2D) messaging access
 - DESCRIPTION: Application administrators shall be able to call direct C2D methods and send C2D messages to all user devices
 - RATIONALE: Administrators must be able to halt erroneous device messaging to prevent data corruption or service interruption from bad D2C messages
 - DEPENDENCIES: TBD
 
#### Unregistered User

1. 
 - ID: FR-URUSER
 - TITLE: Read-Only Form access
 - DESCRIPTION: Unregistered users shall have acces to all forms tagged for read-only access
 - RATIONALE: The application will be open to the public to view and request access - forms advertising the applications services, and offering registration will be tagged for read-only access and accessible by unregisterred users
 - DEPENDENCIES: TBD
 
#### Registered Users

1. 
 - ID: FR-USER01
 - TITLE: Registered User Form Access
 - DESCRIPTION: Users shall have access to all forms tagged for User-Access and read-only access
 - RATIONALE: The application allows users to view and interact with forms designed for broad user access (ie - user-relevant updates/news/data features/etc...)
 - DEPENDENCIES: TBD
 
2. 
 - ID: FR-USER02
 - TITLE: User Device Registration
 - DESCRIPTION: Users shall have the ability to request registration of their devices with the application
 - RATIONALE: The application will allow the user to request registration of a device using a registration form - the inputs of the form is accesible by an administrator, who validates the form requirements and provides the user with an access key for their device.
 - DEPENDANCIES: TBD

3. 
 - ID: FR-USER03
 - TITLE: User C2D access
 - DESCRIPTION: Users shall have the ability to send C2D messages to devices registered to them, through a C2D form within the application
 - RATIONALE: A principle function of the application - controlling IoT nodes via a web application from anywhere
 - DEPENDENCIES: FR-USER02
 
4. 
 - ID: FR-USER04
 - TITLE: User IoT database form access
 - DESCRIPTION: Users shall have the ability to access the database modeling form and pull queries from the Azure SQL Server associated with their device - each device will have its own relation within the SQL DB accessible by its associated user
 - RATIONALE: A principle function of the application - the user accesses current and historic data submitted by their IoT nodes
 - DEPENDANCIES: FR-USER02
 
5. 
 - ID: FR-USER05
 - TITLE: User help ticket request form access
 - DESCRIPTION: Users shall have access to IoT-specific help-ticket submission forms, embeded within the IoT DB form
 - Rationale: Users submit help tickets when they percieve the application to be functioning improperly
 - DEPENDANCIES: FR-USER04