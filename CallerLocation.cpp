//Unfinished.

/*
 * This is an idea to provide incoming/outgoing call number location on phone UI,
 * useful for chinese users, and maybe for i11n users.
 * All rights reserved by anphorea@gmail.com, obtain an email confirmation before you copy it.
 */

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
    bool success = QObject::connect(phone, SIGNAL(callUpdated(bb::system::phone::Call&)), this,
            SLOT(onCallUpdated(const Call)));

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

void Service::onCallUpdated(const Call call)
{
    CallState::Type state = call.callState();
    qDebug() << "call updated: callId=" << call.callId() << " callState=" << state;
    qDebug() << "Number is : " << call.phoneNumber();

    if (bb::system::phone::CallState::Incoming == state) {
        qDebug() << "call incoming, Searching for contacts.";

        ContactSearchFilters filters;
        filters.setLimit(1);
        filters.setSearchValue(call.phoneNumber());
        QList<Contact> contactList = bb::pim::contacts::ContactService().searchContacts(filters);
        if (contactList.size() > 0) {
            //found contact, modify it's location info
            Contact m = contactList.at(0);
            qDebug() << "Contact Found : " << m.displayName();

            QList<ContactAttribute> phonelist = m.filteredAttributes(AttributeKind::Phone);
            foreach (const ContactAttribute &pattr, phonelist){
            if (pattr.value().compare(call.phoneNumber())==0) {
                ContactBuilder ccb=m.edit();
                QString originalName=pattr.label();
                QString originalValue=pattr.value();
                ccb.deleteAttribute(pattr);
                ccb.addAttribute(ContactAttributeBuilder().setKind(AttributeKind::Phone)
                        .setLabel(tr("anpho.test"))
                        .setValue(originalValue));
                qDebug()<<"Information Modified.";
                break;
            }
            bb::pim::contacts::ContactService().updateContact(m);
            qDebug()<<"Contact Updated.";
        }
    } else {
        qDebug()<<"Incoming call not found in Contacts.";

        bb::pim::contacts::ContactBuilder cb;
        cb.addAttribute(ContactAttributeBuilder()
                .setKind(AttributeKind::Phone)
                .setSubKind(AttributeSubKind::PhoneMobile)
                .setLabel(tr("anpho.testElse"))
                .setValue(call.phoneNumber()));
        cb.addAttribute(ContactAttributeBuilder()
                .setKind(AttributeKind::Name)
                .setValue(tr("Unknown")));
        Contact createdContact =bb::pim::contacts::ContactService().createContact(cb,false);
        qDebug()<<"Adding Fake Contact";
        QTimer::singleShot(5000, this, SLOT(deleteTempAccount(createdContact)));
    }
    // Do something with this call now that you know
    // that it is connected and active.
}
}

void Service::deleteTempAccount(const bb::pim::contacts::Contact& tc)
{
    bb::pim::contacts::ContactService().deleteContact(tc.accountId());
}
