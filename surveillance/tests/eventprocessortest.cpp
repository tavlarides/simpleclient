#include <QtTest>

#include "processing/eventprocessor.h"

class EventProcessorTest : public QObject {
    Q_OBJECT

private slots:
    void createsHighSeverityIncidentInsideRestrictedZone();
    void rejectsMalformedEvents();
};

void EventProcessorTest::createsHighSeverityIncidentInsideRestrictedZone() {
    EventProcessor processor;
    QSignalSpy incidentSpy(&processor, &EventProcessor::incidentReady);

    processor.process(R"({"eventId":"event-1","cameraId":"camera-7","eventType":"motion","occurredAt":"2026-07-30T20:00:00Z","latitude":37.975,"longitude":23.733,"confidence":0.98})",
                      QStringLiteral("cameras/camera-7/events"));

    QCOMPARE(incidentSpy.count(), 1);
    const surveillance::Incident incident = qvariant_cast<surveillance::Incident>(incidentSpy.takeFirst().at(0));
    QCOMPARE(incident.cameraId, QStringLiteral("camera-7"));
    QCOMPARE(incident.zoneId, QStringLiteral("restricted-perimeter"));
    QCOMPARE(incident.severity, QStringLiteral("high"));
}

void EventProcessorTest::rejectsMalformedEvents() {
    EventProcessor processor;
    QSignalSpy rejectedSpy(&processor, &EventProcessor::rejected);

    processor.process(R"({"eventId":"event-1"})", QStringLiteral("cameras/camera-7/events"));

    QCOMPARE(rejectedSpy.count(), 1);
}

QTEST_MAIN(EventProcessorTest)
#include "eventprocessortest.moc"
