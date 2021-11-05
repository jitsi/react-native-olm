import * as Olm from '@jitsi/react-native-olm'

export class User {
    initializeAccount() {
        this._olmAccount = new Olm.Account();
        this._olmAccount.create();
    }

    initializeSession() {
        this._olmSession = new Olm.Session();
        this._olmSession.create();
    }

    generateKeys() {
        const identityKeys = this._olmAccount.identity_keys();
        const identityKeysJSON = JSON.parse(identityKeys);
        const idKey = identityKeysJSON.curve25519;

        this._olmAccount.generate_one_time_keys(1);
        const oneTimeKeys = this._olmAccount.one_time_keys();
        const oneTimeKeysJSON = JSON.parse(oneTimeKeys);
        const otKey = Object.values(oneTimeKeysJSON.curve25519)[0];
        this._olmAccount.mark_keys_as_published();

        return { idKey, otKey };
    }

    encryptText(plainText, {idKey, otKey}) {
        this._olmSession.create_outbound(this._olmAccount, idKey, otKey);
        return this._olmSession.encrypt(plainText);
    }

    decryptText(encryptedText) {
        const encryptedTextJSON = JSON.parse(encryptedText)
        this._olmSession.create_inbound(this._olmAccount, encryptedTextJSON.body);
        this._olmAccount.remove_one_time_keys(this._olmSession);
        return this._olmSession.decrypt(encryptedTextJSON.type, encryptedTextJSON.body);
    }
}