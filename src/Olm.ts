interface AccountHostObject {
    identity_keys();
    generate_one_time_keys(numberOfKeys);
    one_time_keys();
    mark_keys_as_published();
    remove_one_time_keys(session);
    free();
}

interface SessionHostObject {
    create_outbound(account, idKey, otKey);
    create_inbound(account, message);
    decrypt(type, message);
    encrypt(message);
    free();
}

export function init() {}

export class Account {
    _olmAccount: AccountHostObject;

    create() {
        this._olmAccount = global._olm.createOlmAccount();
    }

    identity_keys() {
        return this._olmAccount?.identity_keys();
    }

    remove_one_time_keys(session) {
        this._olmAccount.remove_one_time_keys(session._olmSession);
    }

    generate_one_time_keys(numberOfKeys) {
        this._olmAccount?.generate_one_time_keys(numberOfKeys);
    }

    one_time_keys(){
        return this._olmAccount?.one_time_keys();
    }

    mark_keys_as_published() {
        this._olmAccount?.mark_keys_as_published();
    }

    free() {
        this._olmAccount.free();
    }
}

export class Session {
    _olmSession: SessionHostObject;

    create() {
        this._olmSession = global._olm.createOlmSession();
    }

    create_outbound(account, idKey, otKey) {
        this._olmSession.create_outbound(account._olmAccount, idKey, otKey);
    }

    create_inbound(account, message) {
        this._olmSession.create_inbound(account._olmAccount, message);
    }

    decrypt(type, message) {
        return this._olmSession.decrypt(type, message);
    }

    encrypt(message) {
        return this._olmSession.encrypt(message);
    }

    free() {
        this._olmSession.free();
    }
}