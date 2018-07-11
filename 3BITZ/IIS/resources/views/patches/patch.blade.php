@extends('layouts.app')

@section('content')
    <div class="container">
        <div class="row">
            <div class="col-md-8 col-md-offset-2">
                <div class="panel panel-default">
                    <div class="panel-heading">Patch</div>

                    <div class="panel-body">
                        @if (!$patch)
                            <h2>This patch does not exist!</h2>
                        @else
                            <form class="form-horizontal" role="form" method="POST"
                                  action="{{ action('PatchController@store') }}">
                                {{ csrf_field() }}

                                <div class="form-group">
                                    <label class="col-md-4 control-label">ID of patch</label>
                                    <div class="col-md-6">
                                        <div class="radio">
                                            <input type="text" class="form-control" name="id"
                                                   value="{{$patch->id_patch}}" readonly>
                                        </div>
                                    </div>
                                </div>

                                <div class="form-group">
                                    <label class="col-md-4 control-label">Approved</label>
                                    <div class="col-md-6">
                                        @if($patch->schvalenie)
                                            <input name="written" class="form-control" value="True" disabled>
                                        @else
                                            <input name="written" class="form-control" value="False" disabled>
                                        @endif
                                    </div>
                                </div>
                                <div class="form-group">
                                <div>
                                    <label class="col-md-4 control-label">Written by</label>
                                    <div class="col-md-6">
                                        <input id="fix_link" name="written" class="form-control" value="{{$patch->name_vydany}}"
                                               readonly onclick="location.href='/profile/{{$patch->name_vydany}}'">
                                    </div>
                                </div>
                                </div>

                                @if($patch->name_schvaleny)
                                    <div class="form-group">
                                        <label class="col-md-4 control-label">Approved by</label>
                                        <div class="col-md-6">
                                            <input for="name_schvaleny" class="form-control"
                                                   id="fix_link"
                                                   readonly value="{{$patch->name_schvaleny}}"
                                                   onclick="location.href='/profile/{{$patch->name_schvaleny}}'"/>
                                        </div>
                                    </div>
                                @endif

                                <div class="form-group">
                                    <label class="col-md-4 control-label">Release date</label>
                                    <div class="col-md-6">
                                        <input type="date" class="form-control" name="datum_vydania"
                                               value="{{$patch->datum_vydania}}" disabled>
                                    </div>
                                </div>

                                @if($patch->schvalenie)
                                    <div class="form-group">
                                        <label class="col-md-4 control-label">Integration date</label>
                                        <div class="col-md-6">
                                            <input type="date" class="form-control" name="datum_zavedenia"
                                                   value="{{$patch->datum_zavedenia}}">
                                        </div>
                                    </div>

                                @endif

                                <div class="form-group">
                                    <label class="col-md-4 control-label">Diff</label>
                                    <div class="col-md-6">
                                        <textarea name="diff" class="form-control" rows="15"
                                                  disabled>{{ $patch->diff }}</textarea>
                                    </div>
                                </div>


                                <hr>

                                <label class="col-md-4 control-label">Test for this patch</label>
                                <div class="col-md-6">
                                    <table class="table table-bordered" id="dataTable" width="100%" cellspacing="0">
                                        <thead>
                                        <tr>
                                            <th>Test</th>
                                            <th>Start date</th>
                                            <th>End date</th>
                                            <th>Result</th>
                                            <th>Tested patch</th>
                                            <th>Approved by</th>
                                        </tr>
                                        </thead>

                                        <tbody>
                                        @foreach($tests as $test)
                                            <tr>
                                                <td class="col-md-1"><a
                                                            href="/tests/{{$test->id}}"> {{ $test->id }} </a></td>
                                                <td class="col-md-1"> {{ $test->datum_zaciatku }} </td>
                                                <td class="col-md-1"> {{ $test->datum_konca }} </td>
                                                <td class="col-md-1"> {{ $test->hodnotenie }} </td>
                                                <td class="col-md-1"><a
                                                            href="/patches/{{$test->id_patch}}">{{$test->id_patch}}</a>
                                                </td>
                                                <td> {{ $test->name_schvaleny }} </td>
                                            </tr>
                                        @endforeach
                                        </tbody>
                                    </table>
                                </div>


                                @if(Auth::user()->group == 'Admin' or Auth::user()->group == 'Programmer')

                                    <div class="col-md-6 col-md-offset-4">
                                        @if(!$patch->schvalenie)
                                            <button name="approve" class="btn btn-primary"
                                                    value="{{Auth::user()->name}}">Approve this patch
                                            </button>
                                        @else
                                        <button type="submit" class="btn btn-primary">Submit changes</button>
                                        <button type="reset" class="btn btn-default">Reset changes</button>
                                        @endif
                                    </div>
                                @endif
                                @endif
                            </form>

                    </div>
                </div>
            </div>
        </div>
    </div>

@endsection