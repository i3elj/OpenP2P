#ifndef G_TEST_H
#define G_TEST_H

#include <QObject>

class G_Test : public QObject
{
    Q_OBJECT
public:
    explicit G_Test(QObject *parent = nullptr);

signals:
};

#endif // G_TEST_H
