import * as React from 'react';
import { View, Text, Button, TextInput } from 'react-native';
import { User } from './User';

let message = "";

export default function App() {
  const [encryptedTextMessage,setEncryptedTextMessage] = React.useState();
  const [decryptedTextMessage,setDecryptedTextMessage] = React.useState();

  const user1 = new User();
  const user2 = new User();

  user1.initializeAccount();
  user2.initializeAccount();

  user1.initializeSession();
  user2.initializeSession();

  const keys = user1.generateKeys();

  return (
    <View style={{ flex: 1,
                   alignItems: 'center',
                   justifyContent: 'center'}}>
      <TextInput 
        placeholder = "Type message here"
        onChangeText = {(text) => { 
          message = text; }}
      />
      <Button style={{ flex: 1 }}
        title = "Send Message"
        onPress = { () => {
          const encryptedText = user2.encryptText(message, keys);
          setEncryptedTextMessage("Message received: " + encryptedText);

          const decryptedText = user1.decryptText(encryptedText);
          setDecryptedTextMessage("Message decoded: " + decryptedText);
        }}
        />
      <Text style={{ color: "black",  margin: 20 }} >
          {encryptedTextMessage}
      </Text>
      <Text style={{color: "black", margin: 20 }} >
          {decryptedTextMessage}
      </Text>
    </View>
  );
}
