#ifndef TOOLKIT_H
#define TOOLKIT_H
//#include <QValidator>

//class QDateTimeEdit;
class QWidget;
class QTreeWidget;
class QString;

void selectTreeItemUp(QTreeWidget* tree);
void selectTreeItemDown(QTreeWidget* tree);
void selectTreeItemFirst(QTreeWidget* tree);
void selectTreeItemLast(QTreeWidget* tree);

void showWarning(QWidget* parent, const QString& caption , const QString& text);

//class StartEndDateTimeValidator : public QValidator
//{
//    Q_OBJECT
//public:
//    StartEndDateTimeValidator(
//            QDateTimeEdit& start,
//            QDateTimeEdit& end,
//            QObject* parent = nullptr);

//};

#endif // TOOLKIT_H
