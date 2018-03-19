/*
 * Copyright (c) Institute for System Programming of the Russian Academy of Sciences
 * All Rights Reserved
 *
 * Institute for System Programming of the Russian Academy of Sciences (ISP RAS)
 * 25 Alexander Solzhenitsyn st., Moscow, 109004, Russia
 * http://www.ispras.ru
 *
 * The present file was automatically generated on the basis of formal specifications.
 * It is distributed under the same terms and conditions as the derived specifications.
 *
 * N.B. PLEASE DO NOT MODIFY THIS FILE.
 */

package ru.ispras.microtesk.model.x86.op;

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.memory.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.x86.PE;
import ru.ispras.microtesk.model.x86.TempVars;
import ru.ispras.microtesk.model.memory.Location;

import static ru.ispras.microtesk.model.x86.TypeDefs.*;

public final class jp extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "jp",
          jp.class,
          null
          );
      addArgument("rel", SBYTE);
      addShortcut(new Info_instruction(), "#root");
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final Immediate rel = (Immediate) getArgument("rel", args);

      return new jp(rel);
    }
  }
  private static final class Info_instruction extends IsaPrimitiveInfoAnd {
    Info_instruction() {
      super(
          IsaPrimitiveKind.OP,
          "jp",
          instruction.class,
          null
          );
      addArgument("rel", SBYTE);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final Immediate rel = (Immediate) getArgument("rel", args);

      return new instruction(
        new jp(
          rel
        )
      );
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final Immediate rel;

  public jp(final Immediate rel) {
    assert SBYTE.equals(rel.getType());

    this.rel = rel;

    addArgument("rel", rel);
  }

  @Override
  public void init(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    return String.format("jp <label>%d", rel.access().load().bigIntegerValue());
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    return String.format("01111010%s", rel.access().load().toString());
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    if (pe__.PF.access().load().equals(Data.valueOf(BIT, 0x1))) {
      pe__.BRANCH.access().store(Data.valueOf(BIT, 0x1));
      pe__.NEXTPC.access().store(Data.signExtend(WORD, rel.access().load()));
    } else {
      vars__.instruction_image_size.access().store(Data.valueOf(SIZE, 0x2));
    }
  }
}