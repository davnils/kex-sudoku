Module SudoGen
    Public Sudoku As New List(Of Square)
    Dim r As New Random

    Public Sub GenerateGrid()
        Clear()
        Dim Squares(80) As Square 'an arraylist of squares: see line 86
        Dim Available(80) As List(Of Integer) 'an arraylist of generic lists (nested lists)
        'we use this to keep track of what numbers we can still use in what squares
        Dim c As Integer = 0 'use this to count the square we are up to

        For x As Integer = 0 To Available.Length - 1
            Available(x) = New List(Of Integer)
            For i As Integer = 1 To 9
                Available(x).Add(i)
            Next
        Next

        Do Until c = 81 'we want to fill every square object with values
            If Not Available(c).Count = 0 Then 'if every number has been tried and failed then backtrack
                Dim i As Integer = GetRan(0, Available(c).Count - 1)
                Dim z As Integer = Available(c).Item(i)
                If Conflicts(Squares, Item(c, z)) = False Then 'do a check with the proposed number
                    Squares(c) = Item(c, z) 'this number works so we add it to the list of numbers
                    Available(c).RemoveAt(i) 'we also remove it from its individual list
                    c += 1 'move to the next number
                Else
                    Available(c).RemoveAt(i) ' this number conflicts so we remove it from its list
                End If
            Else
                For y As Integer = 1 To 9 'forget anything about the current square
                    Available(c).Add(y)   'by resetting its available numbers
                Next
                Squares(c - 1) = Nothing 'go back and retry a different number 
                c -= 1                    'in the previous square
            End If
        Loop
        Dim j As Integer ' this produces the output  list of squares
        For j = 0 To 80
            Sudoku.Add(Squares(j))
        Next
        'This algorithm produces a Sudoku in an average of 0.02 seconds, tested over 5000 iterations
    End Sub

    Public Sub Clear()
        Sudoku.Clear()
    End Sub

    Private Function GetRan(ByVal lower As Integer, ByVal upper As Integer) As Integer
        GetRan = r.Next(lower, upper + 1)
    End Function

    Private Function Conflicts(ByVal CurrentValues As Square(), ByVal test As Square) As Boolean

        For Each s As Square In CurrentValues
            If (s.Across <> 0 And s.Across = test.Across) OrElse _
               (s.Down <> 0 And s.Down = test.Down) OrElse _
               (s.Region <> 0 And s.Region = test.Region) Then

                If s.Value = test.Value Then
                    Return True
                End If
            End If
        Next

        Return False
        Return False
    End Function

    Public Structure Square
        Dim Across As Integer
        Dim Down As Integer
        Dim Region As Integer
        Dim Value As Integer
        Dim Index As Integer
    End Structure

    Private Function Item(ByVal n As Integer, ByVal v As Integer) As Square
        n += 1
        Item.Across = GetAcrossFromNumber(n)
        Item.Down = GetDownFromNumber(n)
        Item.Region = GetRegionFromNumber(n)
        Item.Value = v
        Item.Index = n - 1
    End Function

    Public Function GetAcrossFromNumber(ByVal n As Integer) As Integer
        Dim k As Integer
        k = n Mod 9
        If k = 0 Then Return 9 Else Return k
    End Function

    Public Function GetDownFromNumber(ByVal n As Integer) As Integer
        Dim k As Integer
        If GetAcrossFromNumber(n) = 9 Then
            k = n \ 9
        Else
            k = n \ 9 + 1
        End If
        Return k
    End Function

    Private Function GetRegionFromNumber(ByVal n As Integer) As Integer
        Dim k As Integer
        Dim a As Integer = GetAcrossFromNumber(n)
        Dim d As Integer = GetDownFromNumber(n)

        If 1 <= a And a < 4 And 1 <= d And d < 4 Then
            k = 1
        ElseIf 4 <= a And a < 7 And 1 <= d And d < 4 Then
            k = 2
        ElseIf 7 <= a And a < 10 And 1 <= d And d < 4 Then
            k = 3
        ElseIf 1 <= a And a < 4 And 4 <= d And d < 7 Then
            k = 4
        ElseIf 4 <= a And a < 7 And 4 <= d And d < 7 Then
            k = 5
        ElseIf 7 <= a And a < 10 And 4 <= d And d < 7 Then
            k = 6
        ElseIf 1 <= a And a < 4 And 7 <= d And d < 10 Then
            k = 7
        ElseIf 4 <= a And a < 7 And 7 <= d And d < 10 Then
            k = 8
        ElseIf 7 <= a And a < 10 And 7 <= d And d < 10 Then
            k = 9
        End If
        Return k
    End Function

End Module
