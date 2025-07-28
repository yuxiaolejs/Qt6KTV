# Use Node.js LTS image
FROM node:20-alpine

# Create app directory
WORKDIR /usr/src/app

# Install dependencies
COPY package*.json ./
RUN npm install

# Copy app source
COPY . .

# Expose the port your app uses
EXPOSE 18010

# Create credentials.json with proper escaping for JSON and environment variables
RUN echo '{ \
  "sql": { \
    "host": "db", \
    "user": "root", \
    "password": "my-secret-pw", \
    "database": "media" \
  } \
}' > credentials.json

# Start app
CMD ["node", "app.js"]