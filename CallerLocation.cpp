//Working.

/*
 * This is an idea to provide incoming/outgoing call number location on phone UI,
 * useful for chinese users, and maybe for i11n users.
 * All rights reserved by anphorea@gmail.com, obtain an email confirmation before you copy it.
 */

// ================== HEADER ====================

#ifndef SERVICE_H_
#define SERVICE_H_

#include <QObject>
#include <bb/system/phone/Call>
#include <bb/system/phone/Phone>
#include <bb/pim/contacts/ContactService>

namespace bb
{
    class Application;
}

class Service: public QObject
{
    Q_OBJECT
public:
    Service();
    virtual ~Service()
    {
    }
private slots:
    void onCallUpdated(const bb::system::phone::Call &call);
    void deleteTempAccount();
private:
    bb::system::phone::Phone* phone;
    bb::pim::contacts::Contact createdContact;
    bb::pim::contacts::Contact originalContact;
};

#endif /* SERVICE_H_ */

//============================ BODY =============================


#include "service.hpp"
#include <QObject>
#include <bb/Application>
#include <bb/system/InvokeManager>
#include <bb/system/phone/Call>
#include <bb/system/phone/Phone>
#include <bb/system/phone/CallState>
#include <bb/system/phone/CallType>
#include <bb/pim/contacts/ContactService>
#include <bb/pim/contacts/ContactBuilder>
#include <bb/pim/contacts/ContactAttribute>
#include <bb/pim/contacts/ContactAttributeBuilder>
#include <QTimer>

using namespace bb::system::phone;
using namespace bb::pim::contacts;

Service::Service() :
        QObject()
{
    phone = new bb::system::phone::Phone();
    bool success = connect(phone, SIGNAL(callUpdated(const bb::system::phone::Call&)), this,
            SLOT(onCallUpdated(const bb::system::phone::Call&)));

    if (success) {
        // Signal was successfully connected.
        qDebug() << "CallerLoc Service Hooked.";
    } else {
        qDebug() << "CallerLoc Service Failed to hook.";
        // Failed to connect to signal.
        // This is not normal in most cases and can be a critical
        // situation for your app! Make sure you know exactly why
        // this has happened. Add some code to recover from the
        // lost connection below this line.
    }
}

void Service::onCallUpdated(const bb::system::phone::Call &call)
{
    CallState::Type state = call.callState();
    qDebug() << "call updated: callId=" << call.callId() << " callState=" << state;
    qDebug() << "Number is : " << call.phoneNumber();

    if (bb::system::phone::CallState::Incoming == state && call.phoneNumber().length() > 0) {
        qDebug() << "call incoming, Searching for contacts.";

        ContactSearchFilters filters;
        filters.setLimit(1);
        filters.setSearchValue(call.phoneNumber());
        QList<Contact> contactList = bb::pim::contacts::ContactService().searchContacts(filters);
        if (contactList.size() > 0) {
            //found contact, modify it's location info
            Contact m = contactList.at(0);

            m = bb::pim::contacts::ContactService().contactDetails(m.id());
            qDebug() << "Contact Found : " << m.displayName();
            originalContact = m; // save original contact info.
            qDebug() << "Modifing Contact";

            QList<ContactAttribute> namelist = m.filteredAttributes(AttributeKind::Name);
            bool changed = false;
            foreach (const ContactAttribute &pattr, namelist ){
            if (pattr.subKind() == AttributeSubKind::NameSuffix) {
                if (pattr.value().contains("(")) {
                    qDebug() << "Already modified";
                    changed=true;
                } else {
                    QString v=pattr.value();
                    m.edit().addAttribute(ContactAttributeBuilder()
                            .setKind(AttributeKind::Name)
                            .setSubKind(AttributeSubKind::NameSuffix)
                            .setValue("(" + tr("TEST") + ")"));
                    changed=true;
                }
                break;
            }
        }
            if (!changed) {
                m.edit().addAttribute(
                        ContactAttributeBuilder().setKind(AttributeKind::Name).setSubKind(
                                AttributeSubKind::NameSuffix).setValue("(" + tr("TEST") + ")"));
            // use Suffix, or Prefix if user like.
                changed = true;
            }

            qDebug() << "Information Modified.";
            bb::pim::contacts::ContactService().updateContact(m);
        }
    }
    // Do something with this call now that you know
    // that it is connected and active.
}

void Service::deleteTempAccount()
{
    bb::pim::contacts::ContactService().deleteContact(createdContact.id());
}
