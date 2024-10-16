// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include <QtTest/QtTest>
#include <QtCore/QString>

#include "QtInterfaceFramework/private/qifqueryparser_p.h"

// sadly this has to be a define for QVERIFY2() to work
#define CHECK_ERRORSTRING(_actual_errstr, _expected_errstr) do { \
    if (_expected_errstr.startsWith(QLatin1String("~"))) { \
        QRegularExpression re(QStringLiteral("\\A") + _expected_errstr.mid(1) + QStringLiteral("\\z")); \
        QVERIFY2(re.match(_actual_errstr).hasMatch(), \
                 qPrintable("\n    Got     : " + _actual_errstr.toLocal8Bit() + \
                            "\n    Expected: " + _expected_errstr.toLocal8Bit())); \
    } else { \
        QCOMPARE(_actual_errstr, _expected_errstr); \
    } \
} while (false)

class TestQueryParser: public QObject
{
    Q_OBJECT
private slots:
    void validQueries_data();
    void validQueries();
    void invalidQueries_data();
    void invalidQueries();
    void identifierList_data();
    void identifierList();
    void invalidIdentifierList_data();
    void invalidIdentifierList();
};

void TestQueryParser::validQueries_data()
{
    QTest::addColumn<QString>("query");
    QTest::addColumn<QString>("representationQuery");

    const QLatin1String floatRegExp("[0-9]*([eE][-+]?[0-9]+)?");

    QTest::newRow("= string \" ") << "blubb='foo'" << "blubb=foo";
    QTest::newRow("= string \" escaped") << "blubb=\"foo\"" << "blubb=foo";
    QTest::newRow("= string '") << "blubb='foo'" << "blubb=foo";
    QTest::newRow("= string ' escaped") << "blubb=\'foo\'" << "blubb=foo";
    QTest::newRow("== string \" ") << "blubb=='foo'" << "blubb=foo";
    QTest::newRow("== string \" escaped") << "blubb==\"foo\"" << "blubb=foo";
    QTest::newRow("== string '") << "blubb=='foo'" << "blubb=foo";
    QTest::newRow("== string ' escaped") << "blubb==\'foo\'" << "blubb=foo";
    QTest::newRow("!= string \" ") << "blubb!='foo'" << "blubb!=foo";
    QTest::newRow("!= string \" escaped") << "blubb!=\"foo\"" << "blubb!=foo";
    QTest::newRow("!= string '") << "blubb!='foo'" << "blubb!=foo";
    QTest::newRow("!= string ' escaped") << "blubb!=\'foo\'" << "blubb!=foo";
    QTest::newRow("~= string \" ") << "blubb~='foo'" << "blubb~=foo";
    QTest::newRow("~= string \" escaped") << "blubb~=\"foo\"" << "blubb~=foo";
    QTest::newRow("~= string '") << "blubb~='foo'" << "blubb~=foo";
    QTest::newRow("~= string ' escaped") << "blubb~=\'foo\'" << "blubb~=foo";
    QTest::newRow("> int") << "foo>5" << "";
    QTest::newRow(">= int") << "foo>=5" << "";
    QTest::newRow("= int") << "foo=5" << "";
    QTest::newRow("== int") << "foo==5" << "foo=5";
    QTest::newRow("< int") << "foo<5" << "";
    QTest::newRow("<= int") << "foo<=5" << "";
    QTest::newRow("!= int") << "foo!=5" << "";
    QTest::newRow("> -int") << "foo>-5" << "";
    QTest::newRow(">= -int") << "foo>=-5" << "";
    QTest::newRow("= -int") << "foo=-5" << "";
    QTest::newRow("== -int") << "foo==-5" << "foo=-5";
    QTest::newRow("< -int") << "foo<-5" << "";
    QTest::newRow("<= -int") << "foo<=-5" << "";
    QTest::newRow("!= -int") << "foo!=-5" << "";
    QTest::newRow("> float") << "foo>5.1" << "~foo>5." + floatRegExp;
    QTest::newRow(">= float") << "foo>=5.1" << "~foo>=5." + floatRegExp;
    QTest::newRow("= float") << "foo=5.1" << "~foo=5." + floatRegExp;
    QTest::newRow("== float") << "foo==5.1" << "~foo=5." + floatRegExp;
    QTest::newRow("< float") << "foo<5.1" << "~foo<5." + floatRegExp;
    QTest::newRow("<= float") << "foo<=5.1" << "~foo<=5." + floatRegExp;
    QTest::newRow("!= float") << "foo!=5.1" << "~foo!=5." + floatRegExp;
    QTest::newRow("> -float") << "foo>-5.1" << "~foo>-5." + floatRegExp;
    QTest::newRow(">= -float") << "foo>=-5.1" << "~foo>=-5." + floatRegExp;
    QTest::newRow("= -float") << "foo=-5.1" << "~foo=-5." + floatRegExp;
    QTest::newRow("== -float") << "foo==-5.1" << "~foo=-5." + floatRegExp;
    QTest::newRow("< -float") << "foo<-5.1" << "~foo<-5." + floatRegExp;
    QTest::newRow("<= -float") << "foo<=-5.1" << "~foo<=-5." + floatRegExp;
    QTest::newRow("!= -float") << "foo!=-5.1" << "~foo!=-5." + floatRegExp;
    QTest::newRow("!= -5.0e15") << "foo!=-5.0e15" << "~foo!=-5" + floatRegExp;
    QTest::newRow("!= -5.0E15") << "foo!=-5.0E15" << "~foo!=-5" + floatRegExp;
    QTest::newRow("!= -5.0e-15") << "foo!=-5.0e-15" << "~foo!=-5" + floatRegExp;
    QTest::newRow("!= -5.0e+15") << "foo!=-5.0e+15" << "~foo!=-5" + floatRegExp;
    QTest::newRow("!= oct") << "foo!=050" << "foo!=40";
    QTest::newRow("!= 0x15") << "foo!=0x15" << "foo!=21";
    QTest::newRow("!= 0X15") << "foo!=0X15" << "foo!=21";
    QTest::newRow("> int reversed") << "5>foo" << "foo<5";
    QTest::newRow(">= int reversed") << "5>=foo" << "foo<=5";
    QTest::newRow("< int reversed") << "5<foo" << "foo>5";
    QTest::newRow("<= int reversed") << "5<=foo" << "foo>=5";
    QTest::newRow("parenthese") << "(foo!=5)" << "";
    QTest::newRow("(query) & query") << "(foo!=5 || bar>3) & testVal!='test'" << "(foo!=5 | bar>3) & testVal!=test";
    QTest::newRow("(query) & (query)") << "(foo!=5 | bar>3) & (testVal!='test')" << "(foo!=5 | bar>3) & (testVal!=test)";
    QTest::newRow("negation") << "foo!=5 | bar>3 & (testVal!='test')" << "foo!=5 | bar>3 & (testVal!=test)";
    QTest::newRow("negation 2") << "!(!foo!=5 | !bar>3) & !(!testVal!='test')" << "!(!foo!=5 | !bar>3) & !(!testVal!=test)";
    QTest::newRow("negation 3") << "!(foo!=5 | !bar>3) & !(!testVal!='test')" << "!(foo!=5 | !bar>3) & !(!testVal!=test)";
    QTest::newRow("negation 4") << "!(!foo!=5 | !bar>3) & (!testVal!='test')" << "!(!foo!=5 | !bar>3) & (!testVal!=test)";
    QTest::newRow("negation 5") << "!(!foo!=5 | !bar>3) & !(testVal!='test')" << "!(!foo!=5 | !bar>3) & !(testVal!=test)";
    QTest::newRow("negation 6") << "!(!foo!=5 | bar>3) & !(!testVal!='test' | property == 3)" << "!(!foo!=5 | bar>3) & !(!testVal!=test | property=3)";
    QTest::newRow("very complex") << "!(((!foo!=5)) | (bar>3 && foo=\"bar\") & (prop ~= 'foobar') ) & !(!testVal!='test' | property == 3) & test >= 5"
                                  << "!(((!foo!=5)) | (bar>3 & foo=bar) & (prop~=foobar)) & !(!testVal!=test | property=3) & test>=5";
}

void TestQueryParser::validQueries()
{
    QFETCH(QString, query);
    QFETCH(QString, representationQuery);

    if (representationQuery.isEmpty())
        representationQuery = query;

    QIfQueryParser parser;
    parser.setQuery(query);

    QIfAbstractQueryTerm *term = parser.parse();
    QVERIFY2(term, qPrintable(parser.lastError()));

//    if ()
//    QRegularExpression regexp(representationQuery);
//    QRegularExpressionMatch match = regexp.match(term->toString());

//    QVERIFY2(match.hasMatch(), QString(QLatin1String("\nParse result: %1 \nExpected:     %2\n")).arg(term->toString(), representationQuery).toUtf8());

    CHECK_ERRORSTRING(term->toString(), representationQuery);

    //Test the searialize functions
    QByteArray buffer;
    QDataStream in(&buffer, QIODevice::WriteOnly);
    in << term;

    QDataStream out(buffer);
    QIfAbstractQueryTerm *newTerm = nullptr;
    out >> &newTerm;

    QCOMPARE(term->toString(), newTerm->toString());
    delete newTerm;
    delete term;
}

void TestQueryParser::invalidQueries_data()
{
    QTest::addColumn<QString>("query");

    QTest::newRow("string characters mismatch") << "blubb=\"foo'";
    QTest::newRow("~= int") << "blubb~=5";
    QTest::newRow(">= string") << "blubb>='foo'";
    QTest::newRow(">= string") << "blubb>=foo";
    QTest::newRow("== prototype") << "blubb==foo";
    QTest::newRow("wrong separator (locale)") << "blubb=5,1";
    QTest::newRow("wrong formated float") << "blubb==5.0ee1";
    QTest::newRow("parenthese mismatch") << "((blubb=5)";
    QTest::newRow("wrong hex number") << "blubb=0xG";
    QTest::newRow("&& following ||") << "(blubb=5) || && (foo=\"barl\")";
    QTest::newRow("Two parenthesed clauses without conjunction") << "!(!foo!=5 | bar>3) (!'test'!=testVal | property == 3)";
}

void TestQueryParser::invalidQueries()
{
    QFETCH(QString, query);

    QIfQueryParser parser;
    parser.setQuery(query);

    QVERIFY(!parser.parse());
    QVERIFY(!parser.lastError().isEmpty());
}

void TestQueryParser::identifierList_data()
{
    QTest::addColumn<QString>("query");
    QTest::addColumn<QString>("identifier");

    QTest::newRow("< int") << "foo<5" << "foo";
    QTest::newRow("> int reversed") << "5>foo" << "foo";
    QTest::newRow("= string \" ") << "blubb='foo'" << "blubb";
    QTest::newRow("= string \" reversed ") << "'foo'=blubb" << "blubb";
}

void TestQueryParser::identifierList()
{
    QFETCH(QString, query);
    QFETCH(QString, identifier);

    QIfQueryParser parser;
    parser.setQuery(query);

    QSet<QString> set;
    set.insert(identifier);
    parser.setAllowedIdentifiers(set);
    QIfAbstractQueryTerm *term = parser.parse();
    QVERIFY2(term, qPrintable(parser.lastError()));
    delete term;
}

void TestQueryParser::invalidIdentifierList_data()
{
    QTest::addColumn<QString>("query");
    QTest::addColumn<QString>("identifier");

    QTest::newRow("< int") << "foo<5" << "foobar";
    QTest::newRow("> int reversed") << "5>foo" << "foobar";
    QTest::newRow("= string \" ") << "blubb='foo'" << "blubber";
    QTest::newRow("= string \" reversed ") << "'foo'=blubb" << "blubber";
}

void TestQueryParser::invalidIdentifierList()
{
    QFETCH(QString, query);
    QFETCH(QString, identifier);

    QIfQueryParser parser;
    parser.setQuery(query);

    QSet<QString> set;
    set.insert(identifier);
    parser.setAllowedIdentifiers(set);

    QVERIFY(!parser.parse());
    QVERIFY(!parser.lastError().isEmpty());
}

//TODO add autotests for the orderTerms

QTEST_MAIN(TestQueryParser)

#include "tst_queryparser.moc"
