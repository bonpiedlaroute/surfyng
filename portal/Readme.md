# Surfyn Portal


+ **Build portal**

	```bash
	 mvn clean install
  java -jar target/portal-0.0.1-SNAPSHOT.jar
 
 #Build the docker image
 docker build -t surfyn:latest .
 
 #Start Postgres
 sudo docker run -d -t -i -e POSTGRES_DB=portal -e POSTGRES_USER=surfyn -e POSTGRES_PASSWORD=psurfyn -p 5432:5432 --name postgres postgres:9.5.15
 
 #Start Surfyn
 sudo docker run -d -t -i -e ACTIVE_PROFILE=prod -e ACTIVE_CONTEXT=/portal -e POSTGRES_HOST=postgres -e POSTGRES_PORT=5432 -e POSTGRES_USERNAME=surfyn -e POSTGRES_PASSWORD=psurfyn -e GOOGLE_CLIENT_ID=1080663386908-75irf7rtb430o2foidk95j2bloa8ut0s.apps.googleusercontent.com -e GOOGLE_CLIENT_SECRET=KeREL988Gm88IrlOVmoGpvKt -e FACEBOOK_CLIENT_ID=467157600564935 -e FACEBOOK_CLIENT_SECRET=a55890f1cb6ded2276fc844c9fbbbfa2 -p 8080:8080 --link postgres:postgres --name surfyn surfyn:latest
 
 
 #with docker-compose
 docker-compose up -d
 docker-compose stop
	```

+ **Open your web browser, and navigate to http://localhost:8080**
